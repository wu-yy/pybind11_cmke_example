//
// Created by wuyongyu on 2020/5/20.
//
#include "cache_compile.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#ifdef __clang__
    #if !defined(__has_warning) || __has_warning("-Wdefaulted-function-deleted")
    //#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
    #pragma clang diagnostic ignored "-Wdefaulted-function-deleted"
    #endif
#endif
#ifdef __GNUC__

#endif

#include <pybind11/iostream.h>
namespace py = pybind11;

int add(int i, int j) {
    return i + j;
}

PYBIND11_MODULE(utils_core, m) {
    pybind11::add_ostream_redirect(m, "ostream_redirect");
    m.def("cache_compile", &leetcode::jit_compiler::cache_compile);
    m.def("log", [&](const std::string& fileline,
                     char level,
                     int verbose,
                     const std::string& s) {
        // if verbose level not match, return
        if (level=='i' && !(leetcode::FLAGS_log_vprefix.size() ?
                            leetcode::check_vlog(fileline.c_str(), verbose) :
                            verbose <= leetcode::FLAGS_log_v)
                ) {
            return;
        }
        if (level != 'f') {
            leetcode::LogVoidify() &&
                    leetcode::Log(fileline.c_str(), level, verbose) << s;
        } else {
            leetcode::LogFatalVoidify() &&
                    leetcode::Log(fileline.c_str(), level, verbose) << s;
        }
    });
    m.def("log_capture_start", &leetcode::log_capture_start);
    m.def("log_capture_stop", &leetcode::log_capture_stop);
    m.def("log_capture_read", &leetcode::log_capture_read);
}