// SPDX-License-Identifier: MIT
/*
 * git-entry-parser.hh
 *
 * Copyright (C) sw4k
 * 
 * 
 */
#include "git-entry-parser.hh"
namespace csemver
{
    GitEntryParser::GitEntryParser(Configuration *config, Logger *logger)
    {
        _config = config;
        _logger = logger;
    }

    GitEntry GitEntryParser::Parse(std::string input)
    {
        if (input.empty())
        {
            return GitEntry();
        }

        GitEntry entry;        
        std::size_t idxmax = input.size() - 1;
        std::size_t idx1 = 0;
        std::size_t idx2 = input.find('\n', idx1);
        entry.CommitHash = input.substr(idx1, idx2 - idx1);

        idx1 = idx2 + 1;
        idx2 = input.find('\n', idx1);
        auto refs = input.substr(idx1, idx2 - idx1);

        do
        {
            idx1 = idx2 + 1;
            if (idx1 < idxmax)
            {
                idx2 = input.find('\n', idx1);
                auto parsed = input.substr(idx1, idx2 - idx1);
                if (entry.Subject.empty())
                {
                    entry.Subject += parsed;
                }
                else
                {
                    entry.Subject += "\n" + parsed;
                }
            }
        } while (idx2 > idx1);

        do
        {
            idx1 = idx2 + 1;
            if (idx1 < idxmax)
            {
                idx2 = input.find('\n', idx1);
                if (std::string::npos == idx2)
                {
                    idx2 = idxmax + 1;
                }
                auto parsed = input.substr(idx1, idx2 - idx1);
                if (entry.Body.empty())
                {
                    entry.Body += parsed;
                }
                else
                {
                    entry.Body += "\n" + parsed;
                }
            }
        } while (idx2 > idx1);

        do
        {
            idx1 = idx2 + 1;
            if (idx1 < idxmax)
            {
                idx2 = input.find('\n', idx1);
                if (std::string::npos == idx2)
                {
                    idx2 = idxmax + 1;
                }
                auto parsed = input.substr(idx1, idx2 - idx1);
                entry.Footers.push_back(parsed);
            }
        } while (idx2 > idx1);

        if (!refs.empty())
        {
            idx2 = -1;
            idxmax = refs.size() - 1;
            do
            {
                idx1 = idx2 + 1;
                if (idx1 < idxmax)
                {
                    if (idx1 != 0)
                    {
                        idx1++;
                    }
                    idx2 = refs.find(',', idx1);
                    if (std::string::npos == idx2)
                    {
                        idx2 = idxmax + 1;
                    }
                    auto parsed = refs.substr(idx1, idx2 - idx1);
                    entry.Refs.push_back(parsed);
                }
            } while (idx2 > idx1);
        }

        return entry;
    }

    bool GitEntry::empty()
    {
        return 
            CommitHash.empty()
            && Refs.size() == 0
            && Subject.empty()
            && Body.empty()
            && Footers.size() == 0;
    }

    std::string GitEntry::toString()
    {
        if (empty())
        {
            return std::string();
        }
        auto result = CommitHash + "\n";
        if (Refs.size() > 0)
        {
            int refidx = 0;
            for (auto ref : Refs)
            {
                if (refidx == 0)
                {
                    result += ref;
                }
                else
                {
                    result += ", " + ref;
                }
                refidx++;
            }
        }
        result += "\n";
        result += Subject + "\n";
        result += "\n" + Body + "\n";
        if (Footers.size() > 0)
        {
            int footeridx = 0;
            for (auto footer : Footers)
            {
                if (footeridx == 0)
                {
                    result += footer;
                }
                else
                {
                    result += "," + footer;
                }
                footeridx++;
            }
        }
        return result;
    }
}