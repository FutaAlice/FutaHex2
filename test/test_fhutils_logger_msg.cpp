#include <thread>
#include <list>
#include <gtest/gtest.h>
#include <fhutils/logger.h>

TEST(fhutils_logger, msg) {
    using namespace std;
    using namespace fhutils::logger;

    list<shared_ptr<thread>> oThreads;
    for (int i = 0; i < 10; i++) {
        oThreads.push_back(shared_ptr<thread>(new thread([=]() {
            for (int j = 0; j < 10; ++j)
                msg() << "Thread " << i << ", Message " << j;
        })));
    }
    for (int i = 0; i < 100; i++)
        msg() << "Main thread, Message " << i;
    for (auto oThread : oThreads)
        oThread->join();

    msg(Level::Info) << "output to console, done.";
    msg(Level::Info) << "press any to continue this test.";
    SUCCEED();
}