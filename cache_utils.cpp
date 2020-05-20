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

//PYBIND11_MODULE(utils_core, m)
//{
//    // optional module docstring
//    m.doc() = "pybind11 example plugin";
//    // expose add function, and add keyword arguments and default arguments
//    m.def("add", &add, "A function which adds two numbers", py::arg("i")=1, py::arg("j")=2);
//
//    // exporting variables
//    m.attr("the_answer") = 42;
//    py::object world = py::cast("World");
//    m.attr("what") = world;
//}
 PYBIND11_MODULE(utils_core, m) {
    pybind11::add_ostream_redirect(m, "ostream_redirect");
    m.def("cache_compile", &leetcode::jit_compiler::cache_compile);
    m.def("log", [&](const std::string& fileline,
                     char level,
                     int verbose,
                     const std::string& s) {
//         return if verbose level not match
//        if (level=='i' && !(teaflow::log_vprefix.size() ?
//                            teaflow::check_vlog(fileline.c_str(), verbose) :
//                            verbose <= teaflow::log_v)
//                ) {
//            return;
//        }
//        if (level != 'f') {
//            teaflow::LogVoidify() &&
//            teaflow::Log(fileline.c_str(), level, verbose) << s;
//        } else {
//            teaflow::LogFatalVoidify() &&
//            teaflow::Log(fileline.c_str(), level, verbose) << s;
//        }
    });
//    m.def("log_capture_start", &teaflow::log_capture_start);
//    m.def("log_capture_stop", &teaflow::log_capture_stop);
//    m.def("log_capture_read", &teaflow::log_capture_read);
}