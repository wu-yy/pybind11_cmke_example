//
// Created by wuyongyu on 2020/5/24.
//

#pragma once
#include <Python.h>
#include "common.h"

namespace leetcode {

struct PyArgPrinter {
    PyObject* obj;
    const char* name;
};
std::ostream& operator<<(std::ostream& os, const PyArgPrinter& arg);

struct PyTupleArgPrinter {
    PyObject* obj;
    const char* name;
};
std::ostream& operator<<(std::ostream& os, const PyTupleArgPrinter& args);

struct PyKwArgPrinter {
    PyObject* obj;
};
std::ostream& operator<<(std::ostream& os, const PyKwArgPrinter& args);

struct PyFastCallArgPrinter {
    PyObject** obj;
    int64 n;
    PyObject* kw;
};
std::ostream& operator<<(std::ostream& os, const PyFastCallArgPrinter& args);

}
