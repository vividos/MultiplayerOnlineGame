#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <iterator>
#include <boost/limits.hpp>
#include <boost/integer.hpp>
#include <boost/xint/integer.hpp>
#include <boost/static_assert.hpp>

namespace xopt = boost::xint::options;
using boost::xint::callback_t;
using boost::xint::no_callback;

// Get the number of bits in a character. It's almost certainly eight, but
// "almost certainly" doesn't cut it in portable code. We'll also specify an
// unsigned character instead of a signed one, so we can handle binary data
// safely.
static const std::size_t cbits = std::numeric_limits<unsigned char>::digits;

template <std::size_t Bits>
class Rsa {
  public:
    // We're going to use a fixed-length type for this example, primarily to
    // show how they would be used. The calculations require intermediate
    // results that are between two and three times the bit-size of the number
    // though. We'll also make it unsigned, and use secure mode to reduce the
    // chance that the key data gets written to disk anywhere.
    typedef typename boost::xint::integer_t<xopt::fixedlength<Bits * 3>,
        xopt::secure, xopt::negative_modulus> KeyNumber;

    // The data will rarely be an exact multiple of the number of bytes that the
    // encryption will return, so we need to store the original size of the data
    // to remove the extra bytes. We'll want the storage to take up as few bits
    // as possible too, so this typedef defines the smallest type that can hold
    // a Bits-length number of characters.
    typedef typename boost::uint_value_t<(Bits + cbits - 1) / cbits>::least
        SizeT;

    // We must have enough bits to handle at least one character and a SizeT...
    BOOST_STATIC_ASSERT(Bits > ((sizeof(SizeT) + 1) * cbits));

    // ...and the number of bits needs to be even.
    BOOST_STATIC_ASSERT((Bits & 0x01) == 0);

    Rsa(const std::string keys);

    std::string publickey() const;
    std::string privatekey() const;

    std::string encrypt(const std::string &data) const;
    std::string decrypt(const std::string &data) const;

    static Rsa generate(callback_t callback = no_callback);

  private:
    static std::size_t calculate_blocksize(const KeyNumber &n);

    Rsa(const KeyNumber _n, const KeyNumber _d, const KeyNumber _e);

    std::string number_to_binary_string(const KeyNumber &num) const;
    KeyNumber binary_string_to_number(const std::string &s) const;

    // I would normally use more descriptive variable names, but these are the
    // traditional names for these variables. 'n' is used as the modulus for the
    // public and private keys; 'd' is the decryption (i.e. private) key
    // component; and 'e' is the encryption (i.e. public) component of the key.
    KeyNumber n, d, e;
    std::size_t blocksize;
};

template <std::size_t Bits>
std::size_t Rsa<Bits>::calculate_blocksize(const KeyNumber &n) {
    // Round the size of n (in bits) down to the next lower multiple of the
    // number of bits in a character. That's how many characters we can fit into
    // a single block, for encryption purposes.
    std::size_t size_in_bits = log2(n) - 1;
    return (size_in_bits + cbits - 1) / cbits;
}

template <std::size_t Bits>
std::string Rsa<Bits>::number_to_binary_string(const KeyNumber &num) const {
    // Turn a number into a binary string, using the library's binary_t type as
    // an intermediary.
    boost::xint::binary_t b = to_binary(num);
    std::string s;
    std::copy(b.begin(), b.end(), std::back_inserter(s));
    return s;
}

template <std::size_t Bits>
typename Rsa<Bits>::KeyNumber Rsa<Bits>::binary_string_to_number(const
    std::string &s) const
{
    boost::xint::binary_t b;
    std::copy(s.begin(), s.end(), std::back_inserter(b));
    return KeyNumber(b);
}

template <std::size_t Bits>
Rsa<Bits>::Rsa(const std::string keys) {
    // Make sure it's a proper key, by checking the signature.
    bool goodkey = true;
    if (keys.substr(0, 4) == "{RSA") {
        std::istringstream str(keys.substr(4));
        std::size_t recordedbits = 0;
        char c1 = 0, c2 = 0, c3 = 0, c4 = 0;
        str >> recordedbits >> c1 >> e >> c2 >> n >> c3;
        if (c1 == ',' && c2 == ',') {
            if (c3 == ',') {
                // It's a private key, including the decryption key.
                str >> d >> c4;
                if (c4 != '}') goodkey = false;
            } else {
                // It's a public key, no decryption key is included.
                if (c3 != '}') goodkey = false;
            }
        } else goodkey = false;

        // Make sure it's the right size
        if (goodkey && recordedbits != Bits)
            throw std::out_of_range("Wrong number of bits");
    } else goodkey = false;

    if (!goodkey) throw std::invalid_argument("Not a valid key");
    blocksize = calculate_blocksize(n);
}

template <std::size_t Bits>
Rsa<Bits>::Rsa(const KeyNumber _n, const KeyNumber _d, const KeyNumber _e):
    n(_n), d(_d), e(_e)
{
    blocksize = calculate_blocksize(n);
}

template <std::size_t Bits>
std::string Rsa<Bits>::publickey() const {
    std::ostringstream str;
    str << "{RSA" << Bits << ',' << e << ',' << n << '}';
    return str.str();
}

template <std::size_t Bits>
std::string Rsa<Bits>::privatekey() const {
    std::ostringstream str;
    str << "{RSA" << Bits << ',' << e << ',' << n << ',' << d << '}';
    return str.str();
}

template <std::size_t Bits>
std::string Rsa<Bits>::encrypt(const std::string &data) const {
    // A proper implementation would pad the message with additional random
    // data, to avoid the low encryption exponent attack. This example
    // implementation does not.

    // The message may contain up to (blocksize - 1) extra bytes when it's
    // decrypted. Prepend a SizeT with the number of bytes to remove from the
    // end.
    const unsigned char mask = (unsigned char)(-1);
    std::string msg;
    SizeT trimblock = blocksize - ((data.length() + sizeof(SizeT)) % blocksize);
    if (trimblock == blocksize) trimblock = 0;
    for (std::size_t i = sizeof(SizeT); i > 0; --i) {
        msg += static_cast<char>(trimblock & mask);
        trimblock >>= cbits;
    }
    msg += data;

    // Split the message into blocks of blocksize and encrypt each one.
    std::string encrypted_msg;
    for (std::size_t block = 0; block * blocksize < msg.length(); ++block) {
        // Grab a block of blocksize bytes.
        std::string tblock = msg.substr(block * blocksize, blocksize);

        // Turn it into a KeyNumber.
        KeyNumber mnumber = binary_string_to_number(tblock);

        // Encrypt that number by raising it to the power of 'e' (the public
        // component of the key), modulus the 'n' component.
        mnumber = powmod(mnumber, e, n);

        // Append the encrypted data to the return value, padded to the proper
        // block length.
        tblock = number_to_binary_string(mnumber);
        if (tblock.length() < blocksize) tblock += std::string(blocksize -
            tblock.length(), 0);
        encrypted_msg += tblock;
    }

    return encrypted_msg;
}

template <std::size_t Bits>
std::string Rsa<Bits>::decrypt(const std::string &encrypted_msg) const {
    std::string decrypted_msg;

    // Split the message into blocks of blocksize and decrypt each one.
    for (std::size_t block = 0; block * blocksize < encrypted_msg.length();
        ++block)
    {
        // Grab a block of blocksize bytes.
        std::string tblock = encrypted_msg.substr(block * blocksize, blocksize);

        // Turn it into a KeyNumber.
        KeyNumber mnumber = binary_string_to_number(tblock);

        // Decrypt that number by raising it to the power of 'd' (the private
        // component of the key), modulus the 'n' component.
        mnumber = powmod(mnumber, d, n);

        // Append the encrypted data to the return value, padded to the proper
        // block length.
        tblock = number_to_binary_string(mnumber);
        if (tblock.length() < blocksize) tblock += std::string(blocksize -
            tblock.length(), 0);
        decrypted_msg += tblock;
    }

    // Trim the added bytes off of it.
    SizeT trimblock = 0;
    for (std::size_t i = 0; i < sizeof(SizeT); ++i)
        trimblock |= (SizeT(decrypted_msg[i]) << (i * cbits));
    decrypted_msg = decrypted_msg.substr(sizeof(SizeT), decrypted_msg.length() -
        trimblock - sizeof(SizeT));

    return decrypted_msg;
}

template <std::size_t Bits>
Rsa<Bits> Rsa<Bits>::generate(callback_t callback) {
    // Use the system's strong random number generator, via the XInt-provided
    // convenience class.
    boost::xint::strong_random_generator gen;

    // Generate two random prime numbers, each containing no more than half of
    // the requested bits, and compute the product.
    KeyNumber p = KeyNumber::random_prime(gen, Bits / 2, callback);
    KeyNumber q = KeyNumber::random_prime(gen, Bits / 2, callback);
    assert(p != q); // If they're identical, there's almost certainly a problem

    // Compute the product of the primes.
    KeyNumber n(p * q);

    // Select an encryption key e, such that e and (p - 1) * (q - 1) are
    // relatively prime. Encryption goes a lot faster if you use small primes
    // for this value, and 65537 is recommended by X.509 and PKCS #1.
    KeyNumber e(65537);

    // Compute the decryption key.
    KeyNumber d(invmod(e, (p - 1) * (q - 1)));

    // That's all we have to do. Just plug those numbers into an Rsa object and
    // return it.
    return Rsa<Bits>(n, d, e);
}



using std::cout;
using std::endl;
using std::flush;

const int max_statusline_length = 40;

void clearline(int length) {
    cout << '\r' << std::string(length, ' ') << '\r' << flush;
}

bool callback() {
    static int n = 0;
    if (++n == max_statusline_length) {
        clearline(max_statusline_length);
        n = 0;
    }
    cout << '.' << flush;
    return true;
}

int main() {
    typedef Rsa<512> Cipher;

    // In this test program, we'll generate a new key every time. 512-bit keys
    // should generate within 100 attempts or less, most of the time, so it'll
    // be pretty quick. You would normally generate a new key only once, store
    // the private key securely, and publish the public key.
    Cipher c = Cipher::generate(callback);
    clearline(max_statusline_length);

    std::string source("This is a test message.");
    std::string encrypted = c.encrypt(source);
    std::string decrypted = c.decrypt(encrypted);

    if (decrypted == source) {
        cout << "Decryption was successful!" << endl << endl;
        cout << "The key is: " << c.privatekey() << endl;
    } else {
        cout << "Error in decryption!" << endl;
        cout << source << " (" << source.length() << ')' << endl;
        cout << decrypted << " (" << decrypted.length() << ')' << endl;
    }

    return EXIT_SUCCESS;
}
