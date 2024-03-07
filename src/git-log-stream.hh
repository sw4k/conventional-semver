// SPDX-License-Identifier: MIT
/*
 * git-log-stream.hh
 *
 * Copyright (C) sw4k
 * 
 * 
 */
#ifndef GIT_LOG_STREAM_HH_
#define GIT_LOG_STREAM_HH_

#include <istream>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include "configuration.hh"
#include "git-entry-parser.hh"
#include "logger.hh"
#include "output-generator.hh"

namespace csemver
{
    class GitLogStreamBuf : public std::basic_streambuf<char>
    {
    private:
        char *_buf;
        size_t _bufsize = 1024 * 1024;
        Configuration *_config;
        Logger *_logger;
        std::unique_ptr<std::vector<char>> _nextEntry;
        std::iostream::int_type overflow(std::iostream::int_type ch);
        int sync();
        std::iostream::int_type underflow();
    public:
        GitLogStreamBuf(
            Configuration *config,
            Logger *logger);
        ~GitLogStreamBuf();
        std::string readentry();
    };

    class GitLogStream : public std::basic_iostream<char>
    {
    private:
        Configuration *_config;
        GitEntryParser *_gitEntryParser;
        Logger *_logger;
        std::unique_ptr<GitLogStreamBuf> _streambuf;
    public:
        GitLogStream(
            Configuration *config,
            GitEntryParser *gitEntryParser,
            Logger *logger);
        ~GitLogStream();
        GitEntry readentry();
    };
}

#endif
