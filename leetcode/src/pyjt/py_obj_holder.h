
//
// Created by wuyongyu on 2020/5/24.
//

#pragma once
#include <Python.h>
#include "common.h"

namespace leetcode {
struct PyObjHolder {
    PyObject* obj;
    inline PyObjHolder(PyObject* obj) : obj(obj) {
        if (!obj) {
            PyErr_Print();
            LOGf << "Python error occur";
        }
    }
    inline ~PyObjHolder() {
        if (obj) Py_DECREF(obj);
    }
    inline PyObject* release() {
        auto tmp = obj;
        obj = nullptr;
        return tmp;
    }
};
}

// PyMODINIT_FUNC return PyObject*
#define PYJF_MODULE_INIT(name) \
PyMODINIT_FUNC PyInit_##name() { \
    PyObject *m; \
    try { \
        PyModuleDef *def = new PyModuleDef(); \
        memset(def, 0, sizeof(PyModuleDef)); \
        def->m_name = #name; \
        def->m_doc = ""; \
        def->m_size = -1; \
        Py_INCREF(def); \
        leetcode::PyObjHolder holder(m = PyModule_Create(def)); \
        init_module(def, m); \
        holder.release(); \
    } catch(const std::exception& e) { \
        PyErr_SetString(PyExc_RuntimeError, e.what()); \
        return nullptr; \
    } \
    return m; \
}

#define PYJF_MODULE_INIT_TEST(name) \
PyMODINIT_FUNC PyInit_##name() {\
PyObject *m; \
try { \
        PyModuleDef *def = new PyModuleDef(); \
        memset(def, 0, sizeof(PyModuleDef)); \
        def->m_name = #name; \
        def->m_doc = ""; \
        def->m_size = -1; \
        Py_INCREF(def); \
        m = PyModule_Create(def); \
        init_module_test(def, m); \
    } catch(const std::exception& e) { \
        PyErr_SetString(PyExc_RuntimeError, e.what()); \
        return nullptr; \
    } \
    return m; \
}

