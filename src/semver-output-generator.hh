// SPDX-License-Identifier: MIT
/*
 * semver-output-generator.hh
 *
 * Copyright (C) sw4k
 * 
 * 
 */
#ifndef SEMVER_OUTPUT_GENERATOR_HH_
#define SEMVER_OUTPUT_GENERATOR_HH_

#include <memory>
#include <regex>
#include <string>
#include <tuple>
#include <vector>
#include "configuration.hh"
#include "git-entry-parser.hh"
#include "output-generator.hh"
#include "semver-component-type.hh"

namespace csemver
{
    class SemverOutputGenerator : public OutputGenerator
    {
    private:
        int _major;
        int _minor;
        int _patch;
    public:
        SemverOutputGenerator(Configuration *config, Logger *logger);
        void HandleCommitEntry(GitEntry entry);
        void GenerateOutput();
    };
}

#endif
