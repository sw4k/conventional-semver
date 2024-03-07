// SPDX-License-Identifier: MIT
/*
 * git-entry-parser.hh
 *
 * Copyright (C) sw4k
 * 
 * 
 */
#ifndef GIT_ENTRY_PARSER_HH_
#define GIT_ENTRY_PARSER_HH_

#include <vector>
#include "configuration.hh"
#include "logger.hh"

namespace csemver
{
    class GitEntry
    {
    public:
        std::string CommitHash;
        std::string Body;
        std::vector<std::string> Footers;
        std::vector<std::string> Refs;
        std::string Subject;
        bool empty();
        std::string toString();
    };

    class GitEntryParser
    {
    private:
        Configuration *_config;
        Logger *_logger;
    public:
        GitEntryParser(
            Configuration *config,
            Logger *logger);
        GitEntry Parse(std::string input);
    };
}

#endif
