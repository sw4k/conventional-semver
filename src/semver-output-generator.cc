// SPDX-License-Identifier: MIT
/*
 * semver-output-generator.cc
 *
 * Copyright (C) sw4k
 * 
 * 
 */
#include <iostream>
#include "semver-output-generator.hh"

namespace csemver
{
    SemverOutputGenerator::SemverOutputGenerator(Configuration *config, Logger *logger)
    {
        logger->Trace("ctor");
        _config = config;
        _logger = logger;
        _major = config->MajorStart;
        _minor = config->MinorStart;
        _patch = config->PatchStart;
    }

    void SemverOutputGenerator::HandleCommitEntry(GitEntry entry)
    {
        _logger->Trace("HandleCommitEntry");
        SemverComponentType semverComponent = SemverComponentType::None;
        for (auto item : *_config->Types)
        {
            auto target_component = std::get<1>(item);
            if (target_component > semverComponent)
            {
                auto target_regex = std::get<0>(item);
                if (std::regex_match(entry.Subject, target_regex))
                {
                    semverComponent = target_component;
                }
            }
        }
        for (auto item : *_config->Footers)
        {
            auto target_component = std::get<1>(item);
            for (auto footer : entry.Footers)
            {
                if (target_component > semverComponent)
                {
                    auto target_regex = std::get<0>(item);
                    if (std::regex_match(footer, target_regex))
                    {
                        semverComponent = target_component;
                    }
                }
            }
        }
        switch (semverComponent)
        {
            case SemverComponentType::Major:
                _logger->Debug("major=" + entry.Subject);
                _major += 1;
                _minor = 0;
                _patch = 0;
                break;
            case SemverComponentType::Minor:
                _logger->Debug("minor=" + entry.Subject);
                _minor += 1;
                _patch = 0;
                break;
            case SemverComponentType::Patch:
                _logger->Debug("patch=" + entry.Subject);
                _patch += 1;
                break;
            default:
                _logger->Debug("none=" + entry.Subject);
                break;
        }
    }

    void SemverOutputGenerator::GenerateOutput()
    {
        _logger->Trace("GenerateOutput");
        std::cout << _major << "." << _minor << "." << _patch << std::endl;
    }
}
