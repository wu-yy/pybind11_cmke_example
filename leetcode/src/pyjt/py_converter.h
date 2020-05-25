//
// Created by wuyongyu on 2020/5/24.
//

#pragma once
#include "pyjt/py_obj_holder.h"
#include "pyjt/numpy.h"
#include "common.h"
#include "misc/hash.h"
//#include "misc/nano_string.h"
//#include "misc/fast_shared_ptr.h"

namespace leetcode {

#define DEF_IS(check_type, return_type) \
template<class T> \
typename std::enable_if<std::is_same<T, check_type>::value, return_type>::type

#define GET_PY_NONE(code) ((code), Py_INCREF(Py_None), Py_None)

inline Log& operator<<(Log& os, PyObject* objp) {
    PyObjHolder repr_obj(PyObject_Repr(objp));

    if (PyUnicode_CheckExact(repr_obj.obj)) {
        return os << Py_TYPE(objp)->tp_name <<
                  PyUnicode_AsUTF8(repr_obj.obj);
    } else {
        return os << "unknown(" >> (void*)objp >> ")";
    }
}

// string
DEF_IS(string, bool) is_type(PyObject* obj) {
    return PyUnicode_CheckExact(obj);
}

DEF_IS(string, PyObject*) to_py_object(const string& a) {
    return PyUnicode_FromStringAndSize(a.c_str(), a.size());
}

DEF_IS(string, string) from_py_object(PyObject* obj) {
    Py_ssize_t size;
    const char* s = PyUnicode_AsUTF8AndSize(obj, &size);
    CHECK(s);
    return string(s, size);
}

// bool
DEF_IS(bool, bool) is_type(PyObject* obj) {
    return PyBool_Check(obj) || PyLong_CheckExact(obj);
}

DEF_IS(bool, PyObject*) to_py_object(const T& a) {
    if (a) Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

DEF_IS(bool, T) from_py_object(PyObject* obj) {
    if (PyBool_Check(obj))
        return obj == Py_True;
    return PyLong_AsLong(obj);
}

// int
DEF_IS(int, bool) is_type(PyObject* obj) {
    return PyLong_CheckExact(obj);
}

DEF_IS(int, PyObject*) to_py_object(const T& a) {
    return PyLong_FromLong(a);
}

DEF_IS(int, T) from_py_object(PyObject* obj) {
    return PyLong_AsLong(obj);
}

// size_t
DEF_IS(size_t, bool) is_type(PyObject* obj) {
    return PyLong_CheckExact(obj);
}

DEF_IS(size_t, PyObject*) to_py_object(const T& a) {
    return PyLong_FromUnsignedLongLong(a);
}

DEF_IS(size_t, T) from_py_object(PyObject* obj) {
    return PyLong_AsUnsignedLongLong(obj);
}

// int64
DEF_IS(int64, bool) is_type(PyObject* obj) {
    return PyLong_CheckExact(obj);
}

DEF_IS(int64, PyObject*) to_py_object(const T& a) {
    return PyLong_FromLongLong(a);
}

DEF_IS(int64, T) from_py_object(PyObject* obj) {
    return PyLong_AsLongLong(obj);
}

// float64
DEF_IS(float64, bool) is_type(PyObject* obj) {
    return PyFloat_CheckExact(obj) || PyLong_CheckExact(obj);
}

DEF_IS(float64, PyObject*) to_py_object(const T& a) {
    return PyFloat_FromDouble(a);
}

DEF_IS(float64, T) from_py_object(PyObject* obj) {
    if (PyFloat_CheckExact(obj))
        return PyFloat_AS_DOUBLE(obj);
    return PyLong_AsDouble(obj);
}

//struct Slice;
//// Slice
//DEF_IS(Slice, bool) is_type(PyObject* obj) {
//    // Return true if ob is a slice object; ob must not be NULL.
//    return PySlice_Check(obj);
//}
//DEF_IS(Slice, T) from_py_object(PyObject* obj) {
//    Py_ssize_t start, stop, step;
//    auto slice = (PySliceObject*)obj;
//
//    // Extract the start, stop and step data members
//    // from a slice object as C integers
//    PySlice_Unpack(obj, &start, &stop, &step);
//    return {start, stop, step,
//            (slice->start == Py_None) |
//            ((slice->stop == Py_None) << 1) |
//            ((slice->step == Py_None) << 2)};
//}

#define GET_RAW_PTR(T, obj) ((T*)(((char*)obj) + sizeof(PyObject)))
#define GET_OBJ_FROM_RAW_PTR(obj) ((PyObject*)(((char*)obj) - sizeof(PyObject)))
#define GET_OBJ_SIZE(T) (sizeof(PyObject)+sizeof(T))

// DumpGraphs
//struct DumpGraphs;
//// PyTypeObject : The C structure of the objects used to describe built-in types.
//extern PyTypeObject PyjtDumpGraphs;
//DEF_IS(DumpGraphs, bool) is_type(PyObject* obj) {
//    // Py_TYPE
//    // This macro is used to access the ob_type member of a Python object.
//    // It expands to (((PyObject*)(o))->ob_type)
//    return Py_TYPE(obj) == &PyjtDumpGraphs;
//}
//
//
//DEF_IS(DumpGraphs, PyObject*) to_py_object(T&& a) {
//    // PyObject* _PyObject_New(PyTypeObject *type)
//    // Return value: New reference.
//    PyObjHolder obj(_PyObject_New(&PyjtDumpGraphs));
//    auto ptr = GET_RAW_PTR(T, obj.obj);
//    new (ptr) T();
//    ptr->nodes_info = std::move(a.nodes_info);
//    ptr->inputs = std::move(a.inputs);
//    ptr->outputs = std::move(a.outputs);
//    return obj.release();
//}
//
//DEF_IS(DumpGraphs, const T&) from_py_object(PyObject* obj) {
//    return GET_RAW_PTR(T, obj);
//}

// MemInfo
//struct MemInfo;
//extern PyTypeObject PyjtMemInfo;
//DEF_IS(MemInfo, bool) is_type(PyObject* obj) {
//    return Py_TYPE(obj) == &PyjtMemInfo;
//}
//
//
//DEF_IS(MemInfo, PyObject*) to_py_object(const T& a) {
//    PyObjHolder obj(_PyObject_New(&PyjtMemInfo));
//    auto ptr = GET_RAW_PTR(T, obj.obj);
//    new (ptr) T(a);
//    return obj.release();
//}
//
//DEF_IS(MemInfo, const T&) from_py_object(PyObject* obj) {
//    return GET_RAW_PTR(T, obj);
//}


// NanoString
//struct NanoString;
//extern PyTypeObject PyjtNanoString;
//DEF_IS(NanoString, bool) is_type(PyObject* obj) {
//    return Py_TYPE(obj) == &PyjtNanoString ||
//           // Return true if the object o is a Unicode object,
//           // but not an instance of a subtype.
//           PyUnicode_CheckExact(obj) ||
//           // Return true if the object o is a type object,
//           // but not a subtype of the standard type object.
//           // Return false in all other cases.
//           PyType_CheckExact(obj) ||
//           // jt.float.__name__
//           PyCallable_Check(obj) ||
//           // numpy.dtype.type
//           // This is equivalent to the Python expression hasattr(o, attr_name)
//           PyObject_HasAttrString(obj, "type");
//}
//
//DEF_IS(NanoString, PyObject*) to_py_object(T a) {
//    PyObjHolder obj(_PyObject_New(&PyjtNanoString));
//    auto ptr = GET_RAW_PTR(T, obj.obj);
//    new (ptr) T(a);
//    return obj.release();
//}
//
//DEF_IS(NanoString, T) from_py_object(PyObject* obj) {
//    if (Py_TYPE(obj) == &PyjtNanoString)
//        return *GET_RAW_PTR(T, obj);
//    if (PyUnicode_CheckExact(obj))
//        return T(PyUnicode_AsUTF8(obj));
//    // PyType
//    if (PyType_CheckExact(obj))
//        return T(_PyType_Name((PyTypeObject *)obj));
//    // jt.float.__name__
//    if (PyCallable_Check(obj)) {
//        // PyObject* PyObject_GetAttrString(PyObject *o, const char *attr_name)¶
//        PyObjHolder t(PyObject_GetAttrString(obj, "__name__"));
//        // PyUnicode_AsUTF8 : The return type is now const char * rather of char *
//        return T(PyUnicode_AsUTF8(t.obj));
//    }
//    PyObjHolder t(PyObject_GetAttrString(obj, "type"));
//    CHECK(PyType_CheckExact(t.obj)) << "Not a valid type:" << t.obj;
//    return T(_PyType_Name((PyTypeObject *)t.obj));
//}

// NanoVector
//struct NanoVector;
//extern PyTypeObject PyjtNanoVector;
//DEF_IS(NanoVector, bool) is_type(PyObject* obj) {
//    return Py_TYPE(obj) == &PyjtNanoVector ||
//           PyList_CheckExact(obj) || PyTuple_CheckExact(obj);
//}
//DEF_IS(NanoVector*, bool) is_type(PyObject* obj) {
//    return Py_TYPE(obj) == &PyjtNanoVector;
//}
//
//DEF_IS(NanoVector, PyObject*) to_py_object(T a) {
//    PyObjHolder obj(_PyObject_New(&PyjtNanoVector));
//    auto ptr = GET_RAW_PTR(T, obj.obj);
//    new (ptr) T(a);
//    return obj.release();
//}
//
//DEF_IS(NanoVector*, T) from_py_object(PyObject* obj) {
//    return GET_RAW_PTR(typename std::remove_pointer<T>::type, obj);
//}
//
//DEF_IS(NanoVector, T) from_py_object(PyObject* obj) {
//    if (Py_TYPE(obj) == &PyjtNanoVector)
//        return *GET_RAW_PTR(T, obj);
//    // Py_SIZE: 此宏用于访问Python对象的 ob_size 成员。
//    // (((PyVarObject*)(o))->ob_size)
//    // 返回对象的成员的个数
//    auto size = Py_SIZE(obj);
//    T a;
//    // PySequence_Fast_ITMES : 返回PyObject指针的基础数组
//    auto arr = PySequence_Fast_ITEMS(obj);
//    for (int64 i=0; i<size; i++) {
//        auto oi = arr[i];
//        CHECK(is_type<int64>(oi));
//        a.push_back_check_overflow(from_py_object<int64>(oi));
//    }
//    return a;
//}

// ArrayArgs
//struct ArrayArgs;
//struct VarHolder;
//vector<ArrayArgs> fetch_sync(const vector<VarHolder*>& vh);
//extern PyHeapTypeObject PyjtVarHolder;
//DEF_IS(ArrayArgs, bool) is_type(PyObject* obj) {
//    // PyArray_Type: py array 类型
//    return Py_TYPE(obj) == PyArray_Type ||
//           PyFloat_CheckExact(obj) ||
//           PyLong_CheckExact(obj) ||
//           PyList_CheckExact(obj) ||
//           Py_TYPE(obj) == &PyjtVarHolder.ht_type;
//}
//
//DEF_IS(ArrayArgs, PyObject*) to_py_object(const T& a) {
//    int64 dims[a.shape.size()];
//    for (int i=0; i<a.shape.size(); i++)
//        dims[i] = a.shape[i];
//    // PyArray_SimpleNew 是numpy的C extension
//    // get_typenum  获取numpy的数据类型， 例如NPY_DOUBLE
//    PyObjHolder obj(PyArray_SimpleNew(
//            a.shape.size(),
//            dims,
//            get_typenum(a.dtype)
//    ));
//    // PyArray_Proxy Definition at  file numpy.h
//    auto arr = (PyArray_Proxy*)(obj.obj);
//    int64 size = PyArray_Size(arr);
//    memcpy((void*)arr->data, (void*)a.ptr, size);
//    return obj.release();
//}
//
//DEF_IS(ArrayArgs, T) from_py_object(PyObject* obj) {
//    // PyFloat_CheckExact :
//    // Return true if its argument is a PyFloatObject,
//    // but not a subtype of PyFloatObject
//    if (PyFloat_CheckExact(obj)) {
//        tmp_data.f32 = PyFloat_AS_DOUBLE(obj);
//        return {&tmp_data, 1, ns_float32};
//    }
//    if (PyLong_CheckExact(obj)) {
//        tmp_data.i32 = PyLong_AsLong(obj);
//        return {&tmp_data, 1, ns_int32};
//    }
//    if (Py_TYPE(obj) == &PyjtVarHolder.ht_type) {
//        auto ptr = GET_RAW_PTR(VarHolder, obj);
//        return move(fetch_sync({ptr}).at(0));
//    }
//    if (Py_TYPE(obj) != PyArray_Type) {
//        PyObjHolder holder(PyArray_FROM_O(obj));
//        auto arr = (PyArray_Proxy*)holder.obj;
//        int64 size = PyArray_Size(arr);
//        T args;
//        args.ptr = arr->data;
//        args.shape = vector<int64>(arr->dimensions, arr->dimensions+arr->nd);
//        args.dtype = get_type_str(arr);
//        args.buffer.reset(new char[size]);
//        memcpy((void*)args.buffer.get(), (void*)arr->data, size);
//        return args;
//    }
//    // PyArray_Type
//    auto arr = (PyArray_Proxy*)obj;
//    CHECK(is_c_style(arr));
//    T args;
//    args.ptr = arr->data;
//    if (arr->dimensions)
//        for (int i=0; i<arr->nd; i++)
//            args.shape.push_back(arr->dimensions[i]);
//    else
//        args.shape = 1;
//    args.dtype = get_type_str(arr);
//    return args;
//}

// VarHolder
//struct VarHolder;
// PyTypeObject 和 PyHeapTypeObject
// 区别是：PyTypeObject对象中，对应的操作方法比如nbadd是存放在函数指针tp asnumber中，
// 而这是另外一个结构，无法计算出nbadd相对于PyTypeObject的相对位置。
// 由PyHeapTypeObject可以直接计算相对位置。
// https://www.itcodemonkey.com/article/5247.html
//extern PyHeapTypeObject PyjtVarHolder;
//namespace jit_op_maker { extern VarHolder* array_(ArrayArgs&& args); }
//DEF_IS(VarHolder*, bool) is_type(PyObject* obj) {
//    return Py_TYPE(obj) == &PyjtVarHolder.ht_type ||
//           is_type<ArrayArgs>(obj);
//}
//
//DEF_IS(VarHolder*, PyObject*) to_py_object(T a) {
//    PyObjHolder obj(_PyObject_New(&PyjtVarHolder.ht_type));
//    auto ptr = GET_RAW_PTR(T, obj.obj);
//    // will move and delete a
//    new (ptr) typename std::remove_pointer<T>::type (a);
//    return obj.release();
//}
//
//
//DEF_IS(VarHolder*, T) from_py_object(PyObject* obj) {
//    CHECK(Py_TYPE(obj) == &PyjtVarHolder.ht_type);
//    return GET_RAW_PTR(VarHolder, obj);
//}
//
//DEF_IS(VarHolder*, T) from_py_object(PyObject* obj, unique_ptr<VarHolder>& holder) {
//    if (Py_TYPE(obj) == &PyjtVarHolder.ht_type)
//        return GET_RAW_PTR(VarHolder, obj);
//    auto args = from_py_object<ArrayArgs>(obj);
//    holder.reset(jit_op_maker::array_(move(args)));
//    return holder.get();
//}

//struct DataView;
//DEF_IS(DataView, PyObject*) to_py_object(T a) {
//    auto obj = GET_OBJ_FROM_RAW_PTR(a.vh);
//    int64 dims[a.shape.size()];
//    for (int i=0; i<a.shape.size(); i++)
//        dims[i] = a.shape[i];
//    PyObjHolder oh(PyArray_New(
//            PyArray_Type, // subtype
//            a.shape.size(), // nd
//            dims, // dims
//            get_typenum(a.dtype), // type_num
//            NULL, // strides
//            a.ptr, // data
//            0, // itemsize
//            NPY_ARRAY_C_CONTIGUOUS | NPY_ARRAY_WRITEABLE, // flags
//            NULL // obj
//    ));
//    Py_INCREF(obj);
//    PyObjHolder oh2(obj);
//    ASSERT(PyArray_SetBaseObject(oh.obj, oh2.obj)==0);
//    oh2.release();
//    return oh.release();
//}

#define CHECK_IS_1(check_type) \
    template<typename T> struct is_##check_type : public std::false_type {}; \
    template<typename T> \
    struct is_##check_type<check_type<T>> : public std::true_type {};

#define DEF_IS_1(check_type, return_type) \
    template<class T> \
    typename std::enable_if<is_##check_type<T>::value, return_type>::type

CHECK_IS_1(vector);

DEF_IS_1(vector, bool) is_type(PyObject* obj) {
    return PyList_CheckExact(obj) || PyTuple_CheckExact(obj);
}

DEF_IS_1(vector, PyObject*) to_py_object(const T& a) {
    PyObjHolder list(PyList_New(a.size()));
    for (uint i=0; i<a.size(); i++) {
        PyObject* o = to_py_object<typename T::value_type>(a[i]);
        CHECK(o);
        // PyList_SET_ITEM borrow ownership, we do not hold this
        PyList_SET_ITEM(list.obj, i, o);
    }
    return list.release();
}

DEF_IS_1(vector, PyObject*) to_py_tuple(const T& a) {
    PyObjHolder list(PyTuple_New(a.size()));
    for (uint i=0; i<a.size(); i++) {
        PyObject* o = to_py_object<typename T::value_type>(a[i]);
        CHECK(o);
        // PyTuple_SET_ITEM borrow ownership, we do not hold this
        PyTuple_SET_ITEM(list.obj, i, o);
    }
    return list.release();
}

DEF_IS_1(vector, PyObject*) to_py_object(T&& a) {
    PyObjHolder list(PyList_New(a.size()));
    for (uint i=0; i<a.size(); i++) {
        PyObject* o = to_py_object<typename T::value_type>(std::move(a[i]));
        CHECK(o);
        // PyList_SET_ITEM borrow ownership, we do not hold this
        PyList_SET_ITEM(list.obj, i, o);
    }
    return list.release();
}

DEF_IS_1(vector, T) from_py_object(PyObject* obj) {
    auto size = Py_SIZE(obj);
    T a(size);
    auto arr = PySequence_Fast_ITEMS(obj);
    for (int64 i=0; i<size; i++) {
        auto oi = arr[i];
        CHECK(is_type<typename T::value_type>(oi));
        a[i] = from_py_object<typename T::value_type>(oi);
    }
    return a;
}

//struct FetchFunc;
//
//DEF_IS(FetchFunc, bool) is_type(PyObject* obj) {
//    return PyCallable_Check(obj);
//}
//
//DEF_IS(FetchFunc, T) from_py_object(PyObject* obj) {
//    // PyObject_Call
//    Py_INCREF(obj);
//    T func(
//            // callback
//            [obj](typename T::R* result) {
//                PyObjHolder arrays(to_py_tuple<vector<ArrayArgs>>(result->arrays));
//                PyObjHolder ret(PyObject_Call(obj, arrays.obj, nullptr));
//            },
//            // deleter
//            [obj]() { Py_DECREF(obj); }
//    );
//    return func;
//}

#define CHECK_IS_2(check_type) \
    template<typename T> struct is_##check_type : public std::false_type {}; \
    template<typename Ta, typename Tb> \
    struct is_##check_type<check_type<Ta, Tb>> : public std::true_type {};

#define DEF_IS_2(check_type, return_type) \
    template<class T> \
    typename std::enable_if<is_##check_type<T>::value, return_type>::type

CHECK_IS_2(unordered_map);

DEF_IS_2(unordered_map, bool) is_type(PyObject* obj) {
    return PyDict_CheckExact(obj);
}

DEF_IS_2(unordered_map, PyObject*) to_py_object(const T& a) {
    PyObjHolder dict(PyDict_New());
    for (const auto& kv : a) {
        PyObjHolder key(to_py_object<typename T::key_type>(kv.first));
        PyObjHolder value(to_py_object<typename T::mapped_type>(kv.second));
        PyDict_SetItem(dict.obj, key.obj, value.obj);
    }
    return dict.release();
}

DEF_IS_2(unordered_map, T) from_py_object(PyObject* obj) {
    auto size = Py_SIZE(obj);
    T a;
    a.reserve(size);
    PyObject *key, *value;
    Py_ssize_t pos = 0;
    while (PyDict_Next(obj, &pos, &key, &value)) {
        CHECK(is_type<typename T::key_type>(key)
              && is_type<typename T::mapped_type>(value));
        a.emplace(
                from_py_object<typename T::key_type>(key),
                from_py_object<typename T::mapped_type>(value)
        );
    }
    return a;
}

// copy from unordered_map
CHECK_IS_2(map);

DEF_IS_2(map, bool) is_type(PyObject* obj) {
    return PyDict_CheckExact(obj);
}

DEF_IS_2(map, PyObject*) to_py_object(const T& a) {
    PyObjHolder dict(PyDict_New());
    for (const auto& kv : a) {
        PyObjHolder key(to_py_object<typename T::key_type>(kv.first));
        PyObjHolder value(to_py_object<typename T::mapped_type>(kv.second));
        PyDict_SetItem(dict.obj, key.obj, value.obj);
    }
    return dict.release();
}

DEF_IS_2(map, T) from_py_object(PyObject* obj) {
    T a;
    PyObject *key, *value;
    Py_ssize_t pos = 0;
    while (PyDict_Next(obj, &pos, &key, &value)) {
        CHECK(is_type<typename T::key_type>(key)
              && is_type<typename T::mapped_type>(value));
        a.emplace(
                from_py_object<typename T::key_type>(key),
                from_py_object<typename T::mapped_type>(value)
        );
    }
    return a;
}


//CHECK_IS_1(fast_shared_ptr);
//
//DEF_IS_1(fast_shared_ptr, bool) is_type(PyObject* obj) {
//    return is_type<typename T::value_type>(obj);
//}
//
//DEF_IS_1(fast_shared_ptr, PyObject*) to_py_object(const T& a) {
//    if (a)
//        return to_py_object<typename T::value_type>(a.data());
//    return to_py_object<typename T::value_type>(a);
//}
//
//DEF_IS_1(fast_shared_ptr, T) from_py_object(PyObject* obj) {
//    return from_py_object<typename T::value_type>(obj);
//}


}
