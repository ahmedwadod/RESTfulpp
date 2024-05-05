#ifndef LOGGING_H
#define LOGGING_H

#include <iostream>

namespace RESTfulpp
{

    enum LogLevel
    {
        NONE,
        ERROR,
        INFO,
        DEBUG
    };

    // Global variables
    extern LogLevel g_logLevel;

    // Global functions
    void setLogLevel(LogLevel level);
    void log_e(const std::string &message);
    void log_i(const std::string &message);
    void log_d(const std::string &message);

} // namespace RESTfulpp

#endif // LOGGING_H