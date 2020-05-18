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


int main() {
    std::cout << "this is main cpp" << std::endl;
    std::cout << "end" << std::endl;
    return 0;

}