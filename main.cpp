#include <iostream>
#include<vector>
#include<algorithm>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
using namespace std;
namespace py = pybind11;


class UseType {
public:
    UseType() {}
    ~UseType(){}

private:
    int use;

};

#define DEF_IS(check_type, return_type) \
    template<class T> \
    typename std::enable_if<std::is_same<T, check_type>::value, return_type>::type


extern PyTypeObject PyjtUseType;

#define GET_RAW_PTR(T, obj) ((T*)(((char*)obj) + sizeof(PyObject)))

DEF_IS(UseType, T) from_py_object(PyObject* obj) {
    if (Py_TYPE(obj) == &PyjtUseType)
        return *GET_RAW_PTR(T, obj);
    return T(PyUnicode_AsUTF8(obj));
}

DEF_IS(UseType, bool) is_type(PyObject* obj) {
    return Py_TYPE(obj) == &PyjtUseType ||
           PyUnicode_CheckExact(obj);
}

int add(int i, int j)
{
    return i + j;
}

PYBIND11_MODULE(leetcode, m)
{
    // optional module docstring
    m.doc() = "pybind11 example plugin";
    // expose add function, and add keyword arguments and default arguments
    m.def("add", &add, "A function which adds two numbers", py::arg("i")=1, py::arg("j")=2);

    // exporting variables
    m.attr("the_answer") = 42;
    py::object world = py::cast("World");
    m.attr("what") = world;
}

// should Py_INCREF(Py_None) be required before returning Py_None in C
#define GET_PY_NONE(code) ((code), Py_INCREF(Py_None), Py_None)

extern "C" uint32_t get_tid();

uint32_t get_tid() {
    return (uint32_t)12;
}

void pyjt_def_jit_get_tid(PyObject*m) {
    static PyMethodDef defs[] = {
            {0, 0, 0, 0}
    };
    PyModule_AddFunctions(m, defs);
    // sub module def
    static PyMethodDef submodule_defs[] = {
            {R""(check_empty_block)"",
            (PyCFunction) (PyObject *(*)(PyObject *, PyObject **, int64_t, PyObject *))
                    [](PyObject *self, PyObject **args, int64_t n, PyObject *kw) -> PyObject * {
                        try { ;
                            int64_t arg_filled = 0;
                            (void) arg_filled;

                            if (n <= 0 && n >= 0) { ;;
                                return GET_PY_NONE((get_tid()));
                            }
                        } catch (const std::exception &e) {
                            PyErr_Format(PyExc_RuntimeError, "%s\n%s",
                                         e.what(),
                                         R""(Declarations:
                                  static inline void test_check_empty_block()
                               )""
                            );
                        }
                        return nullptr;
                    },
            METH_FASTCALL | METH_KEYWORDS,
            R""(Document:
 From /home/wuyongyu/.conda/envs/python37/lib/python3.7/site-packages/teaflow-1.0.0-py3.7.egg/tea/src/opt/pass/expand_empty_block_pass.cpp
Declaration:
static inline void test_check_empty_block())""
            },
            {0,0,0,0}

    };
    auto sub = PyImport_AddModule("teaflow_core.tests");
    PyModule_AddFunctions(sub, submodule_defs);
    PyModule_AddObject(m, "tests", sub);

}

int main() {
    std::cout << "this is main cpp" << std::endl;
    std::cout << "end" << std::endl;
    return 0;

}