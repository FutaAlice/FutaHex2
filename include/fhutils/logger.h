#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <mutex>

struct tm;

namespace fhutils {
namespace logger {
    enum class Level { Debug, Warning, Info, Error, Fatal };
    class FileLogger;
    class ConsoleLogger;
    class BaseLogger;
    class BaseLogger {
        class LogStream;
    public:
        BaseLogger() = default;
        virtual ~BaseLogger() = default;
        virtual LogStream operator()(Level nLevel = Level::Debug);
    private:
        const tm *getLocalTime();
        void endline(Level nLevel, std::string &&oMessage);
        virtual void output(const tm *p_tm,
            const char *str_level,
            const char *str_message) = 0;
    private:
        std::mutex lock_;
        tm localTime_;
    };

    class BaseLogger::LogStream : public std::ostringstream {
        BaseLogger &oLogger_;
        Level       nLevel_;
    public:
        LogStream(BaseLogger& oLogger, Level nLevel)
            : oLogger_(oLogger), nLevel_(nLevel) {};
        LogStream(const LogStream& ls)
            : oLogger_(ls.oLogger_), nLevel_(ls.nLevel_) {};
        ~LogStream() {
#if !defined(DEBUG) & !defined(_DEBUG)
            if (Level::Debug == nLevel_) return;
#endif // DEBUG
            oLogger_.endline(nLevel_, std::move(str()));
        }
    };

    class ConsoleLogger : public BaseLogger {
        using BaseLogger::BaseLogger;
        virtual void output(const tm *p_tm,
            const char *str_level,
            const char *str_message);
    };

    extern ConsoleLogger msg;

} // namespace logger
} // namespace fhutils
