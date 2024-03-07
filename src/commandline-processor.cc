// SPDX-License-Identifier: MIT
/*
 * command-line.cc
 *
 * Copyright (C) sw4k
 * 
 * 
 */
#include <iostream>
#include <stdexcept>
//#include <format> // not until gcc-13 is "stable"; https://packages.debian.org/search?keywords=gcc-13
#include "commandline-processor.hh"

#ifndef SEMVER
#define SEMVER "0.0.1"
#endif

namespace csemver
{
    CommandlineProcessor::CommandlineProcessor(Configuration *config, Logger *logger)
    {
        this->_config = config;
        this->_logger = logger;
    }

    void CommandlineProcessor::PrintInfo()
    {
        std::cout << 
            "conventional-semver v" << SEMVER << std::endl << 
            "Copyright (C) sw4k, MIT Licensed" << std::endl;
    }

    void CommandlineProcessor::PrintUsage()
    {
        std::cout << std::endl <<
            "Usage:" << std::endl << 
            "\tconventional-semver [options] [repo-path]" << std::endl << std::endl << 
            "Options:" << std::endl <<
            "\t--help              print usage, then exit." << std::endl <<
            "\t--version           print version info, then exit." << std::endl <<
            "\t--verbose           enable verbose (debug) output." << std::endl <<
            "\t--commit <hash>     indicates which commit hash to start changelog from." << std::endl <<
            "\t--tag <name>        indicates which tag to start changelog from." << std::endl <<
            "\t--changelog <file>  overrides the name of the file the changelog is written to, otherwise 'changelog.md' is the default." << std::endl <<
            "\t--no-semver         disable SEMVER output to STDOUT." << std::endl <<
            "\t--major             SEMVER 'Major' component will start with this value, default is '0'." << std::endl <<
            "\t--minor             SEMVER 'Minor' component will start with this value, default is '0'." << std::endl <<
            "\t--patch             SEMVER 'Patch' component will start with this value, default is '0'." << std::endl <<
            "\t--git-path <path>   overrides the path to `git` tool, otherwise `git` must be in environment PATH." << std::endl <<
            std::endl << "Parameters:" << std::endl <<
            "\trepo-path           the path of the git repository to process, if not specified defaults to working directory." << std::endl << 
            std::endl;
    }

    void CommandlineProcessor::ProcessCommandLine(int argc, char *argv[])
    {
        std::vector<std::string> args;
        for (size_t i = 1; i < argc; i++)
        {
            args.push_back(std::string(argv[i]));
        }
        
        for (size_t i = 0; i < args.size(); i++)
        {
            auto arg = std::string(args[i]);
            if (arg == "--help")
            {
                PrintInfo();    
                PrintUsage();
                exit(0);
            }
            else if (arg == "--version")
            {
                PrintInfo();    
                exit(0);
            }
            else if (arg == "--no-semver")
            {
                this->_config->DisableSemverOutput = true;
            }
            else if (arg == "--changelog")
            {
                i += 1;
                if (i >= args.size() || args[i][0] == '-')
                {
                    throw std::out_of_range("missing argument to `" + arg + "` option");
                }
                this->_config->ChangelogOutputFile = args[i];
            }
            else if (arg == "--commit")
            {
                if (!this->_config->StartTag.empty())
                {
                    throw std::logic_error("cannot specify both `--commit` and `--tag` options.");
                }
                i += 1;
                if (i >= args.size() || args[i][0] == '-')
                {
                    throw std::out_of_range("missing argument to `" + arg + "` option");
                }
                this->_config->StartCommitHash = args[i];
            }
            else if (arg == "--config")
            {
                i += 1;
                if (i >= args.size() || args[i][0] == '-')
                {
                    throw std::out_of_range("missing argument to `" + arg + "` option");
                }
                this->_config->ConfigFile = args[i];
            }
            else if (arg == "--git-path")
            {
                i += 1;
                if (i >= args.size() || args[i][0] == '-')
                {
                    throw std::out_of_range("missing argument to `" + arg + "` option");
                }
                this->_config->GitPath = args[i];
            }
            else if (arg == "--major")
            {
                i += 1;
                if (i >= args.size() || args[i][0] == '-')
                {
                    throw std::out_of_range("missing argument to `" + arg + "` option");
                }
                this->_config->MajorStart = std::stoi(args[i]);
            }
            else if (arg == "--minor")
            {
                i += 1;
                if (i >= args.size() || args[i][0] == '-')
                {
                    throw std::out_of_range("missing argument to `" + arg + "` option");
                }
                this->_config->MinorStart = std::stoi(args[i]);
            }
            else if (arg == "--patch")
            {
                i += 1;
                if (i >= args.size() || args[i][0] == '-')
                {
                    throw std::out_of_range("missing argument to `" + arg + "` option");
                }
                this->_config->PatchStart = std::stoi(args[i]);
            }
            else if (arg == "--tag")
            {
                if (!this->_config->StartCommitHash.empty())
                {
                    throw std::logic_error("cannot specify both `--commit` and `--tag` options.");
                }
                i += 1;
                if (i >= args.size() || args[i][0] == '-')
                {
                    throw std::out_of_range("missing argument to `" + arg + "` option");
                }
                this->_config->StartTag = args[i];
            }
            else if (arg == "--verbose")
            {
                if (this->_logger->GetMinimumLogLevel() == LogLevel::Debug)
                {
                    this->_logger->SetMinimumLogLevel(LogLevel::Trace);
                }
                else
                {
                    this->_logger->SetMinimumLogLevel(LogLevel::Debug);
                }
            }
            else if (this->_config->RepoPath.empty() && arg[0] != '-')
            {
                this->_config->RepoPath = args[i];
            }
            else
            {
                PrintUsage();
                throw std::invalid_argument("unexpected argument: " + arg);
            }
        }
    }
}
