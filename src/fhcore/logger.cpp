#include <cassert>
#include <chrono>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <map>
#include <regex>
#include "logger.h"

using namespace std;
using namespace logger;

ConsoleLogger logger::debug;
FileLogger logger::record("build_at_" __DATE__ "_" __TIME__ ".log");

#ifdef WIN32
#define localtime_r(_Time, _Tm) localtime_s(_Tm, _Time)
#endif

static const map<Level, const char *> LevelStr =
{
    { Level::Debug, "Debug" },
    { Level::Info, "Info" },
    { Level::Warning, "Warning" },
    { Level::Error, "Error" },
    { Level::Fatal, "Fatal" },
};

ostream& operator<< (ostream& stream, const tm* tm)
{
    return stream << 1900 + tm->tm_year << '-'
        << setfill('0') << setw(2) << tm->tm_mon + 1 << '-'
        << setfill('0') << setw(2) << tm->tm_mday << ' '
        << setfill('0') << setw(2) << tm->tm_hour << ':'
        << setfill('0') << setw(2) << tm->tm_min << ':'
        << setfill('0') << setw(2) << tm->tm_sec;
}

BaseLogger::LogStream BaseLogger::operator()(Level nLevel)
{
    return LogStream(*this, nLevel);
}

const tm* BaseLogger::getLocalTime()
{
    auto now = chrono::system_clock::now();
    auto in_time_t = chrono::system_clock::to_time_t(now);
    localtime_r(&in_time_t, &_localTime);
    return &_localTime;
}

void BaseLogger::endline(Level nLevel, string&& oMessage)
{
    _lock.lock();
    output(getLocalTime(), LevelStr.find(nLevel)->second, oMessage.c_str());
    _lock.unlock();
}

void ConsoleLogger::output(const tm *p_tm,
                           const char *str_level,
                           const char *str_message)
{
    cout << '[' << p_tm << ']'
        << '[' << str_level << "]"
        << "\t" << str_message << endl;
    cout.flush();
}

FileLogger::FileLogger(string filename) noexcept
    : BaseLogger()
{
    string valid_filename(filename.size(), '\0');
    regex express("/|:| |>|<|\"|\\*|\\?|\\|");
    regex_replace(valid_filename.begin(),
                  filename.begin(),
                  filename.end(),
                  express,
                  "_");
    _file.open(valid_filename,
               fstream::out | fstream::app | fstream::ate);
    assert(!_file.fail());
}

FileLogger::~FileLogger()
{
    _file.flush();
    _file.close();
}

void FileLogger::output(const tm *p_tm,
                        const char *str_level,
                        const char *str_message)
{
    _file << '[' << p_tm << ']'
        << '[' << str_level << "]"
        << "\t" << str_message << endl;
    _file.flush();
}
