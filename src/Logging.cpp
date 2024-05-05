#include "RESTfulpp/Logging.h"
#include <iostream>
#include <string>

using namespace RESTfulpp;

LogLevel RESTfulpp::g_logLevel = INFO;

void RESTfulpp::setLogLevel(LogLevel level)
{
    RESTfulpp::g_logLevel = level;
}

void RESTfulpp::log_e(const std::string &message)
{
    if (ERROR <= RESTfulpp::g_logLevel)
    {
        std::cout << "[ERROR] " << message << std::endl;
    }
}

void RESTfulpp::log_i(const std::string &message)
{
    if (INFO <= RESTfulpp::g_logLevel)
    {
        std::cout << "[INFO] " << message << std::endl;
    }
}

void RESTfulpp::log_d(const std::string &message)
{
    if (DEBUG <= RESTfulpp::g_logLevel)
    {
        std::cout << "[DEBUG] " << message << std::endl;
    }
}