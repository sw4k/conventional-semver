// SPDX-License-Identifier: MIT
/*
 * program.cc
 * 
 * Copyright (C) sw4k
 * 
 * The main entry point for the `conventional-semver` program.
 * 
 */
#include <filesystem>
#include <iostream>
#include <string>
#include "changelog-output-generator.hh"
#include "commandline-processor.hh"
#include "configuration.hh"
#include "git-adapter.hh"
#include "logger.hh"
#include "output-generator.hh"
#include "semver-output-generator.hh"

int main(int argc, char *argv[])
{
    auto _logger = new csemver::Logger("main");

    // configure
    csemver::Configuration *config = new csemver::Configuration(
        new csemver::Logger("Configuration"));

    // process command-line
    csemver::CommandlineProcessor *commandLine = new csemver::CommandlineProcessor(
        config,
        new csemver::Logger("CommandlineProcessor"));
    commandLine->ProcessCommandLine(argc, argv);

    // process configuration
    config->ProcessConfiguration();

    // allocate output generators
    auto outputGenerators = std::vector<csemver::OutputGenerator *>();
    if (!config->DisableSemverOutput)
    {
        _logger->Debug("SemverOutputGenerator");
        outputGenerators.push_back(new csemver::SemverOutputGenerator(
            config,
            new csemver::Logger("SemverOutputGenerator")));
    }
    if (!config->ChangelogOutputFile.empty())
    {
        _logger->Debug("ChangelogOutputGenerator");
        outputGenerators.push_back(new csemver::ChangelogOutputGenerator(
            config,
            new csemver::Logger("ChangelogOutputGenerator")));
    }

    // process commit history
    auto gitEntryParser = new csemver::GitEntryParser(
        config,
        new csemver::Logger("GitEntryParser"));
    auto gitAdapter = new csemver::GitAdapter(
        config,
        gitEntryParser,
        new csemver::Logger("GitAdapter"),
        outputGenerators);
    gitAdapter->ProcessGitLog();

    // generate outputs
    for (size_t i = 0; i < outputGenerators.size(); i++)
    {
        outputGenerators[i]->GenerateOutput();
    }

    return 0;
}
