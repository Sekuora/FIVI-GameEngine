#include <Log.hpp>

#include <iostream>
#include <sstream>
#include <print>
#include <format>
#include <chrono>

namespace chrono = std::chrono;

using std::cerr;
using std::format;
using std::formatter;
using std::format_parse_context;
using std::format_context;
using std::format_to;
using std::localtime;
using std::println;
using std::put_time;
using std::stringstream;
using std::time_t;
using std::strftime;

string Log::CollectCurrentTime()
{
    // Current Time
    auto now = chrono::system_clock::now();

    // Epoch time
    time_t logTime = chrono::system_clock::to_time_t(now);

    std::string timeStamp(30, '\0');
    strftime(&timeStamp[0], timeStamp.size(), "%y-%m-%d %H:%M:%S", std::localtime(&logTime));

    // Alternative method to capture time as a string - currently comparing performance between implementations

    // // Capture formatted time as timeStamp stringStream
    // stringstream timeStamp;

    // // Epoch time to calendar time and format
    // timeStamp << put_time(localtime(&logTime), "%y-%m-%d %H:%M:%S");

    // return timeStamp.str();

    return timeStamp;
}

void Log::Default(const string &log)
{
    Message message = {log, LOG_Type_Default, Color_Green};

    println("{}", FormatLog(message));
}

void Log::Error(const string &log)
{
    Message message = {log, LOG_Type_Error, Color_Red};
    println(cerr << FormatLog(message));
}

// Message Formatter template
template <>
struct formatter<Message>
{
    constexpr auto parse(format_parse_context &ctx)
    {
        return ctx.begin();
    }

    auto format(const Message &message, format_context &ctx) const
    {
        return format_to(ctx.out(), "{}: {}", message.type, message.log);
    }
};

string Log::FormatLog(Message &message)
{
    // Log Message Default Format
    string FormattedMessage = format("{}[{}] {}{}", message.color, CollectCurrentTime(), message, Color_Default);

    return FormattedMessage;
}

string Log::FormatLog(Message &message, bool useTimestamp)
{
    string FormattedMessage = "";

    if (useTimestamp)
    {
        // Log Message No Timestamp Format
        FormattedMessage = format("{}{}{}", message.color, message.type, message.log, Color_Default);
        return FormattedMessage;
    }
    else
    {
        // Log Message Default Format
        FormattedMessage = format("{}[{}] {}{}", message.color, CollectCurrentTime(), message, Color_Default);
        return FormattedMessage;
    }
}


