//
// Created by wuyongyu on 2020/5/21.
//

#include "flags.h"

namespace leetcode {
    DEFINE_FLAG(int, debug, 3, "debug");
    DEFINE_FLAG(std::string, project_name, "leetcode", "");

    DEFINE_FLAG(int, log_silent, 0, "The log will be completely silent.");
    DEFINE_FLAG(int, log_v, 1, "Verbose level of logging");
    DEFINE_FLAG(int, log_sync, 0, "Set log printed synchronously.");
    DEFINE_FLAG_WITH_SETTER(std::string, log_vprefix, "", "Verbose level of logging prefix example");

    DEFINE_FLAG_WITH_SETTER(std::string, gdb_path, "", "Path of GDB.");
    DEFINE_FLAG(std::string, addr2line_path, "", "Path of addr2line.");
    DEFINE_FLAG(std::string, extra_gdb_cmd, "", "Extra command pass to GDB, seperate by(;) .");
    DEFINE_FLAG(int, has_pybt, 0, "GDB has pybt or not.");
    DEFINE_FLAG(int, trace_depth, 10, "trace depth for GDB.");
    DEFINE_FLAG_WITH_SETTER(int, gdb_attach, 0, "gdb attach self process.");
}