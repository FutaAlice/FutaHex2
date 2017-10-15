#include <string>
#include <list>
#include <thread>
#include <logger.h>
#include "fhcore_test.h"
using namespace std;

int test_fhcore_all()
{
    if (0 != test_fhcore_logger())
        return -1;

    return 0;
}

int test_fhcore_logger()
{
    using namespace logger;
    
    debug() << string(__func__);
    record() << string(__func__);

    return 0;
}