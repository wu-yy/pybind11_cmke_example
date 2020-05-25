//
// Created by wuyongyu on 2020/5/21.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#ifdef __APPLE__
// NOTHING
#else
#include <sys/prctl.h>
#endif

#include <execinfo.h>
#include <iostream>
#include <vector>
#include "log/tracer.h"
#include "flags.h"

using string =  std::string;
namespace leetcode {

    DEFINE_FLAG_WITH_SETTER(string, gdb_path, "", "Path of GDB.");
    DEFINE_FLAG(string, addr2line_path, "", "Path of addr2line.");
    DEFINE_FLAG(string, extra_gdb_cmd, "", "Extra command pass to GDB, seperate by(;) .");
    DEFINE_FLAG(int, has_pybt, 0, "GDB has pybt or not.");
    DEFINE_FLAG(int, trace_depth, 10, "trace depth for GDB.");
    DEFINE_FLAG_WITH_SETTER(int, gdb_attach, 0, "gdb attach self process.");

    string _FLAGS_extra_gdb_cmd;

    void setter_FLAGS_gdb_attach(int v) {
        if (v && FLAGS_gdb_path.size()) {
            static int FLAGS_gdb_attached = 0;
            if (FLAGS_gdb_attached) return;
            FLAGS_gdb_attached = 1;
            // using gdb to print the stack trace
            char pid_buf[30];
            sprintf(pid_buf, "%d", getpid());
            char name_buf[512];
            name_buf[readlink("/proc/self/exe", name_buf, 511)] = 0;
            int child_pid = fork();
            if (!child_pid) {
                LOGi << "gdb attach for" << name_buf << "pid=" >> pid_buf;

                std::vector<const char *> argv{
                        FLAGS_gdb_path.c_str(),
                        "-ex", "catch throw"
                };
                if (auto n = FLAGS_extra_gdb_cmd.size()) {
                    _FLAGS_extra_gdb_cmd = FLAGS_extra_gdb_cmd;
                    _FLAGS_extra_gdb_cmd += '\0';
                    argv.push_back("-ex");
                    argv.push_back(&_FLAGS_extra_gdb_cmd[0]);
                    for (uint i = 0; i < n; i++) {
                        if (_FLAGS_extra_gdb_cmd[i] == ';') {
                            argv.push_back("-ex");
                            _FLAGS_extra_gdb_cmd[i] = '\0';
                            argv.push_back(&_FLAGS_extra_gdb_cmd[i + 1]);
                        }
                    }
                }
                argv.insert(argv.end(), {name_buf, pid_buf, NULL});
                auto ret = execvp(FLAGS_gdb_path.c_str(), (char *const *) &argv[0]);
                LOGf << "execvp failed return" << ret << FLAGS_gdb_path << FLAGS_extra_gdb_cmd;
                exit(1);
            } else {
                // allow children ptrace parent
#ifdef __APPLE__

#else
                prctl(PR_SET_PTRACER, child_pid, 0, 0, 0);
#endif
                // sleep 5s, wait gdb attach
                sleep(5);
            }
        }
    }

    void setter_FLAGS_gdb_path(string
    v) {
    FLAGS_gdb_path = v;
    setter_FLAGS_gdb_attach(FLAGS_gdb_attach);
}

void print_trace() {
    if (FLAGS_gdb_path.size()) {
        // using gdb to print the stack trace
        char pid_buf[30];
        sprintf(pid_buf, "%d", getpid());
        char st_buf[30];
        sprintf(st_buf, "set backtrace limit %d", FLAGS_trace_depth);
        char name_buf[512];
        name_buf[readlink("/proc/self/exe", name_buf, 511)] = 0;
        int child_pid = fork();
        if (!child_pid) {
            std::cerr << "stack trace for " << name_buf << " pid=" << pid_buf << std::endl;

            std::vector<const char *> argv{
                    FLAGS_gdb_path.c_str(), "--batch", "-n",
                    "-ex", "thread",
                    "-ex", st_buf, // "set backtrace limit 10",
                    "-ex", "bt",
            };
            if (FLAGS_has_pybt)
                argv.insert(argv.end(), {"-ex", "set backtrace limit 0", "-ex", "py-bt"});
            if (auto n = FLAGS_extra_gdb_cmd.size()) {
                _FLAGS_extra_gdb_cmd = FLAGS_extra_gdb_cmd;
                _FLAGS_extra_gdb_cmd += '\0';
                argv.push_back("-ex");
                argv.push_back(&_FLAGS_extra_gdb_cmd[0]);
                for (uint i = 0; i < n; i++) {
                    if (_FLAGS_extra_gdb_cmd[i] == ';') {
                        argv.push_back("-ex");
                        _FLAGS_extra_gdb_cmd[i] = '\0';
                        argv.push_back(&_FLAGS_extra_gdb_cmd[i + 1]);
                    }
                }
            }
            argv.insert(argv.end(), {name_buf, pid_buf, NULL});
            execvp(FLAGS_gdb_path.c_str(), (char *const *) &argv[0]);
            exit(0);
        } else {
            // allow children ptrace parent
#ifdef __APPLE__

#else
            prctl(PR_SET_PTRACER, child_pid, 0, 0, 0);
#endif
            waitpid(child_pid, NULL, 0);
        }
    } else {
        void *trace[16];
        char **messages = (char **) NULL;
        int i, trace_size = 0;

        trace_size = backtrace(trace, 16);
        messages = backtrace_symbols(trace, trace_size);
        // skip first stack frame (points here)
        std::cerr << "[bt] Execution path:" << std::endl;
        for (i = 1; i < trace_size; ++i) {
            std::cerr << "[bt] #" << i << " " << messages[i] << std::endl;
            // find first occurence of '(' or ' ' in message[i] and assume
            // everything before that is the file name.
            int p = 0;
            while (messages[i][p] != '(' && messages[i][p] != ' '
                   && messages[i][p] != 0)
                ++p;

            if (!trace[i]) continue;
            if (!FLAGS_addr2line_path.size()) continue;
            char syscom[256];
            sprintf(syscom, "%s %p -f -p -i -e %.*s", FLAGS_addr2line_path.c_str(), trace[i], p, messages[i]);
            //last parameter is the file name of the symbol
            // printf("run '%s'\n", syscom);
            int ret = system(syscom);
            (void) ret;
        }
    }
}

} // end namespace leetcode