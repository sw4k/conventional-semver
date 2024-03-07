// SPDX-License-Identifier: MIT
/*
 * changelog-output-generator.cc
 *
 * Copyright (C) sw4k
 * 
 * 
 */
#include "changelog-output-generator.hh"

namespace csemver
{
    ChangelogOutputGenerator::ChangelogOutputGenerator(Configuration *config, Logger *logger)
    {
        this->_config = config;
        this->_logger = logger;
        logger->Trace(".ctor");
    }

    void ChangelogOutputGenerator::HandleCommitEntry(GitEntry entry)
    {
        this->_logger->Trace("HandleCommitEntry");
        // TODO
        this->_logger->Warn("TODO: HandleCommitEntry");
        //this->_logger->Info(entry);
    }

    void ChangelogOutputGenerator::GenerateOutput()
    {
        this->_logger->Trace("GenerateOutput");
        // TODO
        this->_logger->Warn("TODO: GenerateOutput");
    }
}
