
#include <boost/xint/integer.hpp>
#include <ostream>

int main() {
    using namespace std;
    using namespace boost::xint;

    // We'll wrap this in a try block for demonstration purposes, just to
    // prove that it doesn't throw an exception. You normally don't need a
    // try block if you're using nothrow_integers.
    try {
        // Call the function(s) that may produce errors. The example call tries
        // to interpret an empty string as an integer, which would normally
        // throw an xint::exception::invalid_digit.
        nothrow_integer n = nothrow_integer("");

        // n should be Not-a-Number, because there was an error but exceptions
        // are not allowed on nothrow_integers. This example code is fairly
        // useless, but it shows how to test for errors.
        if (n.is_nan()) {
            cout << "n is Not-a-Number, do some intelligent error handling here."
                << endl;
        } else {
            // This example code should never get to this block, but you would
            // usually put the normal program flow code here.
            cout << "Error, n should be Not-a-Number and it isn't!" << endl;
        }
    } catch (std::exception& e) {
        // This example code should never get to this block, because exceptions
        // were blocked.
        cout << "Error, nothrow_integer threw an exception!\n" << e.what() <<
            endl;
    }
}
