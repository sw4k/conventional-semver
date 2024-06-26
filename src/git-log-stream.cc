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
        if (gptr() == pptr())
        {
            _logger->Trace("underflow-g-eq-p");
            return EOF;
        }
        else if (gptr() < pptr())
        {
            _logger->Trace("underflow-g-lt-p");
            setg(gptr(), gptr(), pptr());
            return *gptr();
        }
        else
        {
            _logger->Trace("underflow-g-gt-p");
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
            _logger->Trace("readentry->reset");
            _nextEntry.reset(new std::vector<char>());
        }
        do
        {
            char ch;
            if (gptr() < egptr())
            {
                ch = *gptr();
            }
            else
            {
	            std::iostream::int_type uf = underflow();
                if (EOF == uf)
                {
                    _logger->Trace("readentry->EOF");
                    return std::string();
                }
                else
                {
                    ch = (char)uf;                    
                }
            }
            if ('\xef' == ch)
            {
                _logger->Trace("readentry->term");
                gbump(1);
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