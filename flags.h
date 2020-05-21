//
// Created by wuyongyu on 2020/5/21.
//

#include "common.h"
#include "mflag/mflag.h"

namespace leetcode {
    DECLARE_FLAG(int, debug);
    DECLARE_FLAG(std::string, project_name);
    DECLARE_FLAG(int, log_silent);
    DECLARE_FLAG(int, log_sync);
    DECLARE_FLAG(int, log_v);
    DECLARE_FLAG(std::string, log_vprefix);

    DECLARE_FLAG(std::string, gdb_path);
    DECLARE_FLAG(std::string, addr2line_path);
    DECLARE_FLAG(std::string, extra_gdb_cmd);
    DECLARE_FLAG(int, has_pybt);
    DECLARE_FLAG(int, trace_depth);
    DECLARE_FLAG(int, gdb_attach);
}