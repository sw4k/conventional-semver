// SPDX-License-Identifier: MIT
/*
 * logger.hh
 *
 * Copyright (C) sw4k
 * 
 * 
 */
#include <iostream>
#include "logger.hh"

namespace csemver
{
    void Logger::LogInternal(LogLevel level, std::string msg)
    {
        if (level >= s_minLogLevel)
        {
            auto line = std::string("[" + _context + "] " + msg);
            if (level >= LogLevel::Warning)
            {
                std::cerr << line << std::endl;
            }
            else
            {
                std::cout << line << std::endl;
            }
        }
    }

    Logger::Logger(std::string context)
    {
        this->_context = context;
    }

    void Logger::Critical(std::string msg)
    {
        this->LogInternal(LogLevel::Critical, msg);
    }

    void Logger::Debug(std::string msg)
    {
        this->LogInternal(LogLevel::Debug, msg);
    }

    void Logger::Error(std::string msg)
    {
        this->LogInternal(LogLevel::Error, msg);
    }

    LogLevel Logger::GetMinimumLogLevel()
    {
        return s_minLogLevel;
    }

    void Logger::Info(std::string msg)
    {
        this->LogInternal(LogLevel::Information, msg);
    }

    void Logger::SetMinimumLogLevel(LogLevel logLevel)
    {
        s_minLogLevel = logLevel;
    }

    void Logger::Trace(std::string msg)
    {
        this->LogInternal(LogLevel::Trace, msg);
    }

    void Logger::Warn(std::string msg)
    {
        this->LogInternal(LogLevel::Warning, msg);
    }
}