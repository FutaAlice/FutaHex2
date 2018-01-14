#include <iostream>
#include "fhcore_test.h"
using namespace std;

template <typename Test, int size>
class fuckTest
{
public:
    int s = size;
};

template <int size>
using fuck = fuckTest<int, size>;

int main()
{
    test_fhcore_all();

    fuck<12> f;
    cout << f.s << endl;

    getchar();
    return 0;
}