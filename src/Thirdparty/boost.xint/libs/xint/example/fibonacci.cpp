#include <iostream>
#include <boost/xint/integer.hpp>

int main() {
    using namespace boost::xint;
    using std::cout;
    using std::endl;

    // Select the limit, in this case the largest number that will fit into an
    // unsigned long.
    integer limit=(std::numeric_limits<unsigned long>::max)();

    cout << "This system supports a " << std::numeric_limits<unsigned
        long>::digits << "-bit unsigned long.\nIts maximum value is " <<
        limit << "." << endl << endl;
    cout << "Searching for the first Fibonacci value that wouldn't fit..." <<
        endl;

    // Set the first two numbers in the sequence.
    integer n, n1=0, n2=1;

    // Now count the items in the sequence as we iterate over them, until we
    // come to the first one greater than the limit we've set.
    size_t counter=0;
    cout << counter++ << ": " << n1 << endl;
    cout << counter++ << ": " << n2 << endl;

    while (1) {
        n = n1 + n2;
        if (n > limit) break;

        // That one didn't do it, go back for another try.
        cout << counter++ << ": " << n << endl;

        n1=n2;
        n2=n;
    }

    cout << endl << "The answer is " << counter << " (" << n << ")." << endl;
}

