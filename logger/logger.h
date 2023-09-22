#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <iostream>
#include <chrono>
#include <queue>
#include <ctime>
#include <fstream>
#include <iomanip>

enum class MessageType {
    Info,
    Warning,
    Error
};

struct logmsg {
    std::chrono::system_clock::time_point timestamp;
    std::string sender;
    MessageType msg_type;
    std::string msg_data;
};


enum class LogMode {
    FileOnly,
    TerminalOnly,
    FileAndTerminal
};

class Logger {
public:
    Logger();
    Logger(const LogMode& log_mode);

    void log(const logmsg& message);
    void processLog(int opt);

private:
    void writeToFile();
    void displayOnTerminal();
    std::string messageTypeToString(MessageType type);

private:
    std::queue<logmsg> logQueue;
    std::ofstream outfile;
    LogMode m_LoggerMode;
};

#endif
