// SPDX-License-Identifier: MIT
/*
 * configuration.hh
 *
 * Copyright (C) sw4k
 * 
 * 
 */
#ifndef CONFIGURATION_HH_
#define CONFIGURATION_HH_

#include <map>
#include <regex>
#include <string>
#include <vector>
#include "logger.hh"
#include "semver-component-type.hh"

namespace csemver
{
    class Configuration
    {
    private:
        Logger *_logger;
        std::regex BuildFooterRegex(std::string input);
        std::regex BuildTypeRegex(std::string input);
        bool CreateEntry(std::string entry, char entryCode);
        bool ProcessConfigurationFile(std::string path);
        void PrintConfigurationSummary();
    public:
        std::string ChangelogOutputFile;
        std::string ConfigFile;
        bool DisableSemverOutput;
        std::unique_ptr<std::vector<std::tuple<std::regex, SemverComponentType>>> Footers;
        std::string GitPath;
        int MajorStart;
        int MinorStart;
        int PatchStart;
        std::string StartCommitHash;
        std::string StartTag;
        std::unique_ptr<std::vector<std::tuple<std::regex, SemverComponentType>>> Types;
        std::string RepoPath;
        Configuration(Logger *logger);
        void ProcessConfiguration();
    };
}

#endif
