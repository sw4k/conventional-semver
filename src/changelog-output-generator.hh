// SPDX-License-Identifier: MIT
/*
 * changelog-output-generator.hh
 *
 * Copyright (C) sw4k
 * 
 * 
 */
#ifndef CHANGELOG_OUTPUT_GENERATOR_HH_
#define CHANGELOG_OUTPUT_GENERATOR_HH_

#include <string>
#include <vector>
#include "configuration.hh"
#include "git-entry-parser.hh"
#include "output-generator.hh"

namespace csemver
{
    class ChangelogOutputGenerator : public OutputGenerator
    {
    private:
        bool _hasStarted;
    public:
        ChangelogOutputGenerator(Configuration *config, Logger *logger);
        void HandleCommitEntry(GitEntry entry);
        void GenerateOutput();
    };
}

#endif
