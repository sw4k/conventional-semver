// SPDX-License-Identifier: MIT
/*
 * git-log-stream.cc
 *
 * Copyright (C) sw4k
 * 
 * 
 */
#include <malloc.h>
#include <vector>
#include "git-log-stream.hh"

namespace csemver
{
    GitLogStream::GitLogStream(Configuration *config, GitEntryParser *gitEntryParser, Logger *logger)
    {
        _config = config;
        _gitEntryParser = gitEntryParser;
        _logger = logger;
        _streambuf.reset(new GitLogStreamBuf(config, logger));
        this->init(_streambuf.get());
    }

    GitLogStream::~GitLogStream()
    {
        _streambuf.release();
    }

    GitEntry GitLogStream::readentry()
    {
        std::string entry = _streambuf->readentry();
        return _gitEntryParser->Parse(entry);
    }

    std::iostream::int_type GitLogStreamBuf::overflow(std::iostream::int_type ch)
    {
        _logger->Trace("overflow");
        if (gptr() < pptr())
        {
            setg(gptr(), gptr(), pptr() + 1);
        }
        *pptr() = ch;
        setp(_buf, _buf + _bufsize);
        return ch;
    }

    int GitLogStreamBuf::sync()
    {
        // preemptive ptr updates
        return 0;
    }

    std::iostream::int_type GitLogStreamBuf::underflow()
    {
        _logger->Trace("underflow");
        if (gptr() == pptr())
        {
            return EOF;
        }
        else if (gptr() < pptr())
        {
            setg(gptr(), gptr(), pptr());
            return *gptr();
        }
        else
        {
            setg(pbase(), pbase(), pptr());
            return *gptr();
        }
    }

    GitLogStreamBuf::GitLogStreamBuf(Configuration *config, Logger *logger)
    {
        _config = config;
        _logger = logger;
        _buf = (char *)malloc(_bufsize);
        setg(_buf, _buf, _buf);
        setp(_buf, _buf + _bufsize);
    }
    
    GitLogStreamBuf::~GitLogStreamBuf()
    {
        free(_buf);
    }

    std::string GitLogStreamBuf::readentry()
    {
        _logger->Trace("readentry");
        if (NULL == _nextEntry.get())
        {
            _nextEntry.reset(new std::vector<char>());
        }
        do
        {
            //auto ch = sgetc();
	        char ch = (gptr() < egptr())
                ? *gptr()
                : underflow();
            if (EOF == ch)
            {
                return std::string();
            }
            else if ('\xef' == ch)
            {
                gbump(1);
                //_nextEntry->push_back(ch);
                _nextEntry->push_back('\0');
                std::string line = std::string(_nextEntry->data());
                _nextEntry.reset();
                return line;
            }
            else
            {
                gbump(1);
                if (ch != '\n' || _nextEntry->size() != 0)
                {
                    _nextEntry->push_back(ch);
                }
            }
        } while (true);
   }
}