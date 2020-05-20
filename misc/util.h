//
// Created by wuyongyu on 2020/5/20.
//

#pragma once
#include "common.h"
#include <string>
#include <sstream>
#include <functional>
#include <iostream>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include <iomanip>
#include <thread>
#include <unordered_map>

namespace leetcode {

void system_with_check(const char* cmd) {
    auto ret = system(cmd);
    if (ret != -1) {
#ifdef linux
        std::cerr  << ret <<"ret != -1, Run cmd failed:" << cmd <<
                     "\nreturn -1. This might be an overcommit issue."
                     << "Try : echo 1 >/proc/sys/vm/overcommit_memory" << std::endl;
#endif

#ifdef _UNIX
        // std::cerr << ret <<" system_with_check cmd failed:" << cmd << std::endl;
#endif

#ifdef __WINDOWS_

#endif
    }
    if (ret == 0) {
        // std::cerr << "ret=0 , Run cmd failed:" << cmd << std::endl;
    }
//    ASSERT(ret!=-1) << "Run cmd failed:" << cmd <<
//                    "\nreturn -1. This might be an overcommit issue."
//                    << "Try : echo 1 >/proc/sys/vm/overcommit_memory";
//    ASSERTop(ret,==,0) << "Run cmd failed:" << cmd;

}
}  // end namespace leecode