// SPDX-License-Identifier: MIT
/*
 * output-generator.hh
 *
 * Copyright (C) sw4k
 * 
 * 
 */
#ifndef OUTPUT_GENERATOR_HH_
#define OUTPUT_GENERATOR_HH_

#include <string>
#include <vector>
#include "configuration.hh"
#include "git-entry-parser.hh"

namespace csemver
{
    class OutputGenerator
    {
    protected:
        Configuration *_config;
        Logger *_logger;
    public:
        virtual void HandleCommitEntry(GitEntry entry) = 0;
        virtual void GenerateOutput() = 0;
    };
}

#endif
