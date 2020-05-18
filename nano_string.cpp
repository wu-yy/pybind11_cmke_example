#include "pyjt/py_converter.h"
#include "common.h"
#include "/home/wuyongyu/.conda/envs/python37/lib/python3.7/site-packages/teaflow-1.0.0-py3.7.egg/tea/src/misc/nano_string.h"

namespace teaflow {

    PyTypeObject PyjtNanoString;

    void pyjt_def_nano_string(PyObject* m) {
        // 在一个 PyMethodDef 结构体中注册需要的函数。
        static PyMethodDef defs[] = {
                {0,0,0,0}
        };
        ASSERT(PyModule_AddFunctions(m, defs)==0);

        static PyMethodDef class_defs[] = {
                {0,0,0,0}
        };
        static PyGetSetDef class_getsets[] = {
                {0,0,0,0}
        };

        static PyNumberMethods number_methods = {0};

        /*
         * PyTypeObject 中有三个很重要的操作族: tp_as_number, tp_as_sequence 和 tp_as_mapping.
           PyNumberMethods *tp_as_number;
            PySequenceMethods *tp_as_sequence;
            PyMappingMethods *tp_as_mapping;
            定义了作为一个数值对象支持的操作. 如果一个对象被视为数值对象(如整型),
            那么它的 tp_as_number.nb_add 就要有对该对象进行加法操作的具体行为
         */
        auto& tp = PyjtNanoString;
        tp.tp_as_number = &number_methods;







        tp.tp_name = "teaflow_core.NanoString";
        tp.tp_basicsize = GET_OBJ_SIZE(NanoString);
        tp.tp_new = PyType_GenericNew;
        tp.tp_flags = Py_TPFLAGS_DEFAULT;

        tp.tp_methods = &class_defs[0];
        tp.tp_getset = &class_getsets[0];

        tp.tp_init =
                [](PyObject* self, PyObject* _args, PyObject* kw) -> int {
                    try {

                        int64 n = Py_SIZE(_args);
                        auto args = (PyObject**)&PyTuple_GET_ITEM(_args, 0);
                        (void)n, (void)args;
                        // TODO: support kw
                        CHECK(kw==0);
                        ;
                        uint64 arg_filled=0;
                        (void)arg_filled;

                        if (n<=1 && n>=1 && PyUnicode_CheckExact(args[0])) {

                            ;
                            const char* arg0 = PyUnicode_AsUTF8(args[0]);

                            CHECK(!PyErr_Occurred());
                            ;
                            ;
                            return (new (GET_RAW_PTR(NanoString,self)) NanoString(arg0),0);
                        }

                        if (n<=1 && n>=1 && is_type<NanoString>(args[0])) {

                            ;
                            NanoString arg0 = from_py_object<NanoString>(args[0]);

                            CHECK(!PyErr_Occurred());
                            ;
                            ;
                            return (new (GET_RAW_PTR(NanoString,self)) NanoString(arg0),0);
                        }

                        LOGf << "Not a valid call";
                    } catch (const std::exception& e) {
                        PyErr_Format(PyExc_RuntimeError, "%s\n%s",
                                     e.what(),
                                     R""(Declarations:
inline NanoString(const char* s)
inline NanoString(const NanoString& other)
)""
                        );
                    }
                    return -1;
                }
                ;

        tp.tp_repr =
                [](PyObject* self) -> PyObject* {
                    try {
                        int64 n = 0; (void)n;;
                        uint64 arg_filled=0;
                        (void)arg_filled;

                        if (n<=0 && n>=0) {
                            ;
                            ;
                            return PyUnicode_FromString(((GET_RAW_PTR(NanoString,self))->to_cstring()));
                        }

                        LOGf << "Not a valid call";
                    } catch (const std::exception& e) {
                        PyErr_Format(PyExc_RuntimeError, "%s\n%s",
                                     e.what(),
                                     R""(Declarations:
inline const char* to_cstring()
)""
                        );
                    }
                    return nullptr;
                }
                ;

        tp.tp_richcompare =
                [](PyObject* self, PyObject* b, int op) -> PyObject* {
                    try {

                        int64 n = 2;
                        PyObject* args[] = {self, b};
                        (void)n, (void)args;
                        ;
                        uint64 arg_filled=0;
                        (void)arg_filled;

                        if (n<=2 && n>=2 && is_type<NanoString>(args[0]) && is_type<NanoString>(args[1]) && op==Py_EQ) {

                            ;
                            NanoString arg0 = from_py_object<NanoString>(args[0]);

                            ;
                            NanoString arg1 = from_py_object<NanoString>(args[1]);

                            CHECK(!PyErr_Occurred());
                            ;
                            ;
                            return to_py_object<bool>((eq(arg0,arg1)));
                        }

                        if (n<=2 && n>=2 && is_type<NanoString>(args[0]) && is_type<NanoString>(args[1]) && op==Py_NE) {

                            ;
                            NanoString arg0 = from_py_object<NanoString>(args[0]);

                            ;
                            NanoString arg1 = from_py_object<NanoString>(args[1]);

                            CHECK(!PyErr_Occurred());
                            ;
                            ;
                            return to_py_object<bool>((ne(arg0,arg1)));
                        }

                        LOGf << "Not a valid call";
                    } catch (const std::exception& e) {
                        PyErr_Format(PyExc_RuntimeError, "%s\n%s",
                                     e.what(),
                                     R""(Declarations:
inline bool eq(const NanoString& a, const NanoString& b)
inline bool ne(const NanoString& a, const NanoString& b)
)""
                        );
                    }
                    return nullptr;
                }
                ;
        ;
        ASSERT(0==PyType_Ready(&tp)) << (PyErr_Print(), 0);
        // python内建了垃圾回收机制, 引用计数是其中的一部分.
        Py_INCREF(&tp);
        // 使用PyModule_AddObject将自定义的对象加入到模块
        ASSERT(0==PyModule_AddObject(m, "NanoString", (PyObject*)&tp));



    }

}