// SPDX-License-Identifier: MIT
/*
 * git-adapter.hh
 *
 * Copyright (C) sw4k
 * 
 * 
 */
#ifndef GIT_ADAPTER_HH_
#define GIT_ADAPTER_HH_

#include <memory>
#include <string>
#include "configuration.hh"
#include "git-log-stream.hh"
#include "logger.hh"
#include "output-generator.hh"

namespace csemver
{
    class GitAdapter
    {
    private:
        Configuration *_config;
        std::unique_ptr<GitLogStream> _gitLogStream;
        Logger *_logger;
        std::vector<OutputGenerator *> _outputGenerators;
        void ConsumeGitOutput(int stdout_fd, int stderr_fd);
        void HandleGitStderr(char *buf, ssize_t count);
        void HandleGitStdout(char *buf, ssize_t count);
    public:
        GitAdapter(
            Configuration *config,
            GitEntryParser *gitEntryParser,
            Logger *logger,
            std::vector<OutputGenerator *> outputGenerators);
        void ProcessGitLog();
    };
}

#endif
