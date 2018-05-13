#include <cassert>
#include <chrono>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <map>
#include "logger.h"

using namespace std;
using namespace fhutils::logger;

ConsoleLogger fhutils::logger::msg;

#ifdef WIN32
#define localtime_r(_Time, _Tm) localtime_s(_Tm, _Time)
#endif

static const map<Level, const char *> LevelStr = {
    { Level::Debug, "Debug" },
    { Level::Info, "Info" },
    { Level::Warning, "Warning" },
    { Level::Error, "Error" },
    { Level::Fatal, "Fatal" },
};

ostream& operator<< (ostream& stream, const tm* tm) {
    return stream << 1900 + tm->tm_year << '-'
        << setfill('0') << setw(2) << tm->tm_mon + 1 << '-'
        << setfill('0') << setw(2) << tm->tm_mday << ' '
        << setfill('0') << setw(2) << tm->tm_hour << ':'
        << setfill('0') << setw(2) << tm->tm_min << ':'
        << setfill('0') << setw(2) << tm->tm_sec;
}

BaseLogger::LogStream BaseLogger::operator()(Level nLevel) {
    return LogStream(*this, nLevel);
}

const tm *BaseLogger::getLocalTime() {
    auto now = chrono::system_clock::now();
    auto in_time_t = chrono::system_clock::to_time_t(now);
    localtime_r(&in_time_t, &localTime_);
    return &localTime_;
}

void BaseLogger::endline(Level nLevel, string&& oMessage) {
    lock_guard<mutex> lock(lock_);
    output(getLocalTime(), LevelStr.find(nLevel)->second, oMessage.c_str());
}

void ConsoleLogger::output(const tm *p_tm,
    const char *str_level,
    const char *str_message) {
    cout << '[' << p_tm << ']'
        << '[' << str_level << "]"
        << "\t" << str_message << endl;
    cout.flush();
}
