#pragma once

#include <LogDefinitions.hpp>

#include <string>
#include <format>
#include <print>

using std::string;
using std::formatter;
using std::println;

/**
 * @brief Message to output in the log.
 * @details Custom struct for logger messages.
 * 
 */
struct Message
{
    string log;
    string type;
    string color;
};


/**
 *  @author Firagic Entertainment
 *  @brief  Log Class. Defines logging features.
 *  @details A log system that attempts
 *  to use fmt C++ 23 features.
*/
class Log
{
public:
    /** Default Message Type */
    static void Default(const string &log);

    /** Error Message Type */
    static void Error(const string &log);

    /** Custom Message Type - Template */
    template<bool useTimestamp, typename... Args>
    static void Custom(const string &log, const string &color, const string &type, const Args &...args);


private:
    /** 
     * @brief Collect and format current system time to a string.
    */
    static string CollectCurrentTime();

    static string FormatLog(Message &message);

    static string FormatLog(Message &message, bool useTimestamp);
};

// Custom Message Template
template<bool useTimestamp, typename... Args>
static void Log::Custom(const string &log, const string &color, const string &type, const Args &...args)
{
    Message message = {log, type, color};
    
   
    string customFormat = format ("{}", FormatLog(message, useTimestamp), message.color, args..., Color_Default);
    println("{}", (customFormat));
}



