// SPDX-License-Identifier: MIT
/*
 * git-adapter.cc
 *
 * Copyright (C) sw4k
 * 
 * 
 */
#include <unistd.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include "git-adapter.hh"

namespace csemver
{
    void GitAdapter::ConsumeGitOutput(int stdout_fd, int stderr_fd)
    {
        _logger->Trace("ConsumeGitOutput");
        fd_set read_fds;
        struct timeval timeout;
        do
        {
            int select_fd;
            FD_ZERO(&read_fds);
            if (stdout_fd < stderr_fd)
            {
                FD_SET(stdout_fd, &read_fds);
                FD_SET(stderr_fd, &read_fds);
                select_fd = stderr_fd;
            }
            else
            {
                FD_SET(stderr_fd, &read_fds);
                FD_SET(stdout_fd, &read_fds);
                select_fd = stdout_fd;
            }
            timeout.tv_sec = 2;
            timeout.tv_usec = 0;
            auto select_result = select(select_fd, &read_fds, NULL, NULL, &timeout);
            if (-1 == select_result)
            {
                throw std::runtime_error("select() failed, aborting.");
            }
            else if (0 == select_result)
            {
                // no more data (timeout)
                _logger->Debug("no more data (timeout)");
                break;
            }
            size_t bufsize = 1024;
            void *buf = malloc(bufsize);
            if (FD_ISSET(stdout_fd, &read_fds))
            {
                _logger->Trace("ConsumeGitOutput::out");
                ssize_t cb1 = read(stdout_fd, buf, bufsize);
                if (cb1 == 0)
                {
                    break;
                }
                HandleGitStdout((char *)buf, cb1);
            }
            if (FD_ISSET(stderr_fd, &read_fds))
            {
                _logger->Trace("ConsumeGitOutput::err");
                ssize_t cb2 = read(stderr_fd, buf, bufsize);
                if (cb2 == 0)
                {
                    break;
                }
                HandleGitStderr((char *)buf, cb2);
            }
        } while (true);
        _logger->Trace("ConsumeGitOutput::end");
    }

    void GitAdapter::HandleGitStderr(char *buf, ssize_t count)
    {
        // NOTE: git-stderr is forwarded to app-stderr for end-user visibility
        write(STDERR_FILENO, buf, count);
    }

    void GitAdapter::HandleGitStdout(char *buf, ssize_t count)
    {
        _gitLogStream->write(buf, count);
        _gitLogStream->flush();
        GitEntry entry = _gitLogStream->readentry();
        while (!entry.empty())
        {
            int count = _outputGenerators.size();
            for (size_t i = 0; i < count; i++)
            {
                auto outputGenerator = _outputGenerators[i];
                outputGenerator->HandleCommitEntry(entry);
            }
            entry = _gitLogStream->readentry();
        }
    }

    GitAdapter::GitAdapter(Configuration *config, GitEntryParser *gitEntryParser, Logger *logger, std::vector<OutputGenerator *> outputGenerators)
    {
        _config = config;
        _gitLogStream.reset(new GitLogStream(config, gitEntryParser, logger));
        _logger = logger;
        std::copy(outputGenerators.begin(), outputGenerators.end(), std::back_inserter(_outputGenerators));
        _logger->Trace(".ctor");
    }

    void GitAdapter::ProcessGitLog()
    {
        this->_logger->Trace("ProcessGitLog");

        const char *argv[8];
        argv[0] = _config->GitPath.c_str();
        argv[1] = "--no-pager";
        argv[2] = "-C";
        argv[3] = _config->RepoPath.c_str();
        argv[4] = "log";
        argv[5] = "--reverse";
        argv[6] = "--format=tformat:%H%n%D%n%B%n%xef";
        argv[7] = NULL;

        int stdin_pipe[2] = { 0 };
        int stdout_pipe[2] = { 0 };
        int stderr_pipe[2] = { 0 };
        pipe(&stdin_pipe[0]);
        pipe(&stdout_pipe[0]);
        pipe(&stderr_pipe[0]);

        pid_t gitPid = fork();
        if (-1 == gitPid)
        {
            throw std::runtime_error("fork() failed");
        }
        else if (0 == gitPid)
        {
            // redirect std streams
            dup2(stdin_pipe[0], STDIN_FILENO);
            dup2(stdout_pipe[1], STDOUT_FILENO);
            dup2(stderr_pipe[1], STDERR_FILENO);
            close(stdin_pipe[0]);
            close(stdin_pipe[1]);
            close(stdin_pipe[1]);

            // exec git
            int exec_result = execvp(argv[0], (char *const *)&argv[0]);
            if (0 > exec_result)
            {
                throw std::runtime_error("exec() failed");
            }
        }
        else
        {
            // close pipe fds "owned" by git process
            close(stdin_pipe[0]);
            close(stdout_pipe[1]);
            close(stderr_pipe[1]);

            ConsumeGitOutput(stdout_pipe[0], stderr_pipe[0]);
            int wait_result = waitpid(gitPid, NULL, 0);
            if (-1 == wait_result)
            {
                auto eno = errno;
                _logger->Error("waitpid error: " + eno);
            }

            // close remaining pipe fds
            close(stdin_pipe[1]);
            close(stdout_pipe[0]);
            close(stderr_pipe[0]);
        }
    }
}