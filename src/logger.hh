// SPDX-License-Identifier: MIT
/*
 * logger.hh
 *
 * Copyright (C) sw4k
 * 
 * 
 */
#ifndef LOGGER_HH_
#define LOGGER_HH_

#include <string>

namespace csemver
{
    enum LogLevel 
    {
        Trace = 0,
        Debug = 1,
        Information = 2,
        Warning = 3,
        Error = 4,
        Critical = 5
    };

    static LogLevel s_minLogLevel = LogLevel::Information;

    class Logger
    {
    private:
        std::string _context;
        void LogInternal(LogLevel logLevel, std::string msg);
    public:
        Logger(std::string context);
        void Critical(std::string msg);
        void Debug(std::string msg);
        void Error(std::string msg);
        LogLevel GetMinimumLogLevel();
        void Info(std::string msg);
        void SetMinimumLogLevel(LogLevel logLevel);
        void Trace(std::string msg);
        void Warn(std::string msg);
    };
}

#endif
