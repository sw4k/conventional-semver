// SPDX-License-Identifier: MIT
/*
 * configuration.cc
 *
 * Copyright (C) sw4k
 * 
 * 
 */
#include <filesystem>
#include <stdexcept>
#include "configuration.hh"
#include <fstream>

namespace csemver
{
    Configuration::Configuration(Logger *logger)
    {
        this->_logger = logger;
        this->Footers.reset(new std::vector<std::tuple<std::regex, SemverComponentType>>);
        this->Types.reset(new std::vector<std::tuple<std::regex, SemverComponentType>>);
    }

    std::regex Configuration::BuildFooterRegex(std::string input)
    {
        return std::regex(
            "[\\S\\s]*" + input + "[\\S\\s]*",
            std::regex_constants::syntax_option_type::_S_ECMAScript
                & std::regex_constants::syntax_option_type::_S_multiline
                & std::regex_constants::syntax_option_type::_S_icase);
    }

    std::regex Configuration::BuildTypeRegex(std::string input)
    {
        return std::regex(
            "^" + input + ":[\\S\\s]*",
            std::regex_constants::syntax_option_type::_S_ECMAScript
                & std::regex_constants::syntax_option_type::_S_multiline
                & std::regex_constants::syntax_option_type::_S_icase);
    }

    bool Configuration::CreateEntry(std::string entry, char entryCode)
    {
        auto idx = entry.find("=");
        if (std::string::npos == idx)
        {
            return false;
        }
        auto regex = BuildTypeRegex(entry.substr(0, idx));
        auto componentType = SemverComponentType::None;
        auto componentCode = std::tolower(entry.substr(idx + 3, 1)[0]);
        switch (componentCode)
        {
            case 'j':
                componentType = SemverComponentType::Major;
                break;
            case 'n':
                componentType = SemverComponentType::Minor;
                break;
            case 't':
                componentType = SemverComponentType::Patch;
                break;
            default:
                return false;
        }
        if ('t' == entryCode)
        {
            Types->push_back(std::tuple{
                regex,
                componentType});
        }
        else if ('f' == entryCode)
        {
            Footers->push_back(std::tuple{
                regex,
                componentType});
        }
        return true;
    }

    bool Configuration::ProcessConfigurationFile(std::string path)
    {
        this->_logger->Trace("ProcessConfigurationFile");
        if (!std::filesystem::exists(path))
        {
            // if config file does not exist, return false
            return false;
        }

        std::ifstream stream(path);
        char entryType = '?';
        while (stream)
        {
            char cline[1024];
            stream.getline(&cline[0], 1024);
            std::string line(cline);
            _logger->Warn(line);
            if (line.empty() || line[0] == '#')
            {
                // ignore empty lines, or lines starting with hash (#)
                continue;
            }
            
            if ("[types]" == line)
            {
                entryType = 't';
            }
            else if ("[footers]" == line)
            {
                entryType = 'f';
            }
            else
            {
                if (!CreateEntry(line, entryType))
                {
                    _logger->Warn("Could not configure entry from: " + line);
                }
                else
                {
                    _logger->Debug("Configured entry for: " + line);
                }
            }
        }

        return true;
    }

    void Configuration::PrintConfigurationSummary()
    {
        this->_logger->Trace("PrintConfigurationSummary");
        if (this->_logger->GetMinimumLogLevel() <= LogLevel::Debug)
        {
            // TODO
            this->_logger->Warn("TODO: PrintConfigurationSummary");
        }
    }

    void Configuration::ProcessConfiguration()
    {
        this->_logger->Trace("ProcessConfiguration");
        bool configWasProcessed = false;
        if (!this->ConfigFile.empty())
        {
            configWasProcessed = this->ProcessConfigurationFile(this->ConfigFile);
            if (!configWasProcessed)
            {
                throw std::logic_error("Failed to process expected configuration file: " + this->ConfigFile);
            }
        }
        else
        {
            std::vector<std::string>(paths);
            paths.push_back(std::string("./conventional-semver.json"));
            paths.push_back(std::string("~/.config/conventional-semver/settings.json"));
            paths.push_back(std::string("/etc/conventional-semver/settings.json"));
            for (size_t i = 0; i < paths.size(); i++)
            {
                configWasProcessed = this->ProcessConfigurationFile(this->ConfigFile);
                if (configWasProcessed)
                {
                    break;
                }
            }
            if (!configWasProcessed)
            {
                // apply defaults (no config file should be required for "standard-compliant behavior")
                this->Types->push_back(std::tuple{
                    BuildTypeRegex(".*!"),
                    SemverComponentType::Major});
                this->Types->push_back(std::tuple{
                    BuildTypeRegex("feat.*"),
                    SemverComponentType::Minor});
                this->Types->push_back(std::tuple{
                    BuildTypeRegex(".*"),
                    SemverComponentType::Patch});
                this->Footers->push_back(std::tuple{
                    BuildFooterRegex("BREAKING[\\-\\.]CHANGE"),
                    SemverComponentType::Major});
            }
        }
        if (this->RepoPath.empty())
        {
            // default to cwd
            this->RepoPath = std::filesystem::current_path();
        }
        if ((!this->StartCommitHash.empty() || !this->StartTag.empty()) && this->ChangelogOutputFile.empty())
        {
            // default to "./changelog.md"
            auto path = std::filesystem::current_path();
            path += "changelog.md";
            this->ChangelogOutputFile = path;
        }
        if (this->GitPath.empty())
        {
            // default to standard install location
            this->GitPath = "git";
        }
        this->PrintConfigurationSummary();
    }
}
