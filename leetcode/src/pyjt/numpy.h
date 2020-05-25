
//
// Created by wuyongyu on 2020/5/24.
//

#pragma once
#include "pyjt/py_obj_holder.h"
#include "common.h"

namespace leetcode {
struct PyArrayDescr_Proxy {
    PyObject_HEAD
            PyObject* typeobj;
    char kind;
    char type;
    char byteorder;
    char flags;
    int type_num;
    int elsize;
    int alignment;
    char* subarray;
    PyObject *fields;
    PyObject *names;
};

struct PyArray_Proxy {
    PyObject_HEAD
    char* data;
    int nd;
    ssize_t* dimensions;
    ssize_t* strides;
    PyObject *base;
    PyArrayDescr_Proxy *descr;
    int flags;
};

#define NPY_ARRAY_C_CONTIGUOUS    0x0001
#define NPY_ARRAY_ALIGNED         0x0100
#define NPY_ARRAY_WRITEABLE       0x0400
// NPY_ARRAY_C_CONTIGUOUS=1
inline bool is_c_style(PyArray_Proxy* obj) { return obj->flags & 1; }

typedef Py_intptr_t npy_intp;

extern unordered_map<string, int> np_typenum_map;

extern void** PyArray_API;
extern PyTypeObject *PyArray_Type;
extern PyTypeObject *PyArrayDescr_Type;
extern PyObject* (*PyArray_New)(PyTypeObject *, int, npy_intp const *, int, npy_intp const *, void *, int, int, PyObject *);
extern PyObject* (*PyArray_FromAny)(PyObject *, PyArrayDescr_Proxy *, int, int, int, PyObject *);
extern unsigned int (*PyArray_GetNDArrayCFeatureVersion)();
extern int (*PyArray_SetBaseObject)(PyObject *arr, PyObject *obj);

#define NPY_ARRAY_ALIGNED         0x0100
#define NPY_ARRAY_WRITEABLE       0x0400
#define NPY_ARRAY_BEHAVED      (NPY_ARRAY_ALIGNED | \
                                NPY_ARRAY_WRITEABLE)

#define NPY_ARRAY_CARRAY       (NPY_ARRAY_C_CONTIGUOUS | \
                                NPY_ARRAY_BEHAVED)

#define PyArray_SimpleNew(nd, dims, typenum) \
        PyArray_New(PyArray_Type, nd, dims, typenum, NULL, NULL, 0, 0, NULL)

#define PyArray_SimpleNewFromData(nd, dims, typenum, data) \
        PyArray_New(&PyArray_Type, nd, dims, typenum, NULL, \
                    data, 0, NPY_ARRAY_CARRAY, NULL)

#define PyArray_FROM_O(m) PyArray_FromAny(m, NULL, 0, 0, 0, NULL)

inline int64 PyArray_Size(PyArray_Proxy* arr) {
    int64 size = 1;
    for (int i=0; i<arr->nd; i++)
        size *= arr->dimensions[i];
    size *= arr->descr->elsize;
    return size;
}

union tmp_data_t {
    int32 i32;
    float32 f32;
};

extern tmp_data_t tmp_data;

void numpy_init();

}