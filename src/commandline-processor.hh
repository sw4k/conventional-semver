// SPDX-License-Identifier: MIT
/*
 * command-line.hh
 *
 * Copyright (C) sw4k
 * 
 * 
 */
#ifndef COMMAND_LINE_HH_
#define COMMAND_LINE_HH_

#include <string>
#include <vector>
#include "configuration.hh"
#include "logger.hh"

namespace csemver
{
    class CommandlineProcessor
    {
    private:
        Configuration *_config;
        Logger *_logger;
        std::vector<std::string> _paths;
    public:
        CommandlineProcessor(
            Configuration *config,
            Logger *logger);
        void PrintInfo();
        void PrintUsage();
        void ProcessCommandLine(int argc, char *argv[]);
    };
}

#endif
