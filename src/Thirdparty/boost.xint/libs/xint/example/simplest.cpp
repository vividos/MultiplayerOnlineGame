#include <iostream>
#include <sstream>
#include <boost/xint/integer.hpp>

int main() {
    using std::cout;
    using std::endl;

    // You can assign an integer type from any built-in integral type, or a
    // constant.
    boost::xint::integer n = 2;

    // You can do any operation with it that you can with the built-in integral
    // types.
    n += 8;    // Will now be 10
    n = n / 2; // Will now be 5
    ++n;       // Will now be 6
    n *= 12;   // Will now be 72
    assert(n == 72);

    // You can write it to a stream, or read it in from one.
    cout << "The number is: " << n << endl;

    std::istringstream stream("-14");
    stream >> n;
    assert(n == -14);

    // And you can do all of this with much larger numbers than the built-in
    // types can handle, too.
    n = boost::xint::integer("123456789012345678901234567890123456789012345");
    cout << "The huge number is: " << n << endl;
    cout << "(That's a " << log2(n) << "-bit number.)" << endl;
}
