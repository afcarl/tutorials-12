#include "class4.hpp"
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

const foo::int_vector& foo::get_list() const {
    return v_;
}

void foo::add(int v) {
    v_.push_back(v);
}

void foo::add(const int_vector& that)
{
    std::copy(std::begin(that), std::end(that), std::back_inserter(v_));
}

namespace
{
    // C++ -> Python
    template<typename T>
    class vector_to_pylist_converter
    {
    public:
        using native_type = T;
        
        static PyObject* convert(const native_type& v)
        {
            namespace py = boost::python;
            py::list retval;
            
            for (const auto& i: v)
            {
                retval.append(py::object(i));
            }
            return py::incref(retval.ptr());
        }
    };
    
    // Python -> C++
    template<typename T>
    class pylist_to_vector_converter
    {
    public:
        using native_type = T;
        
        // Pythonオブジェクトが変換可能か見る。
        static void* convertible(PyObject* pyo)
        {
            if (!PySequence_Check(pyo))
            {
                return nullptr;
            }
            return pyo;
        }
        
        // 実際に変換を行う。
        static void construct(PyObject* pyo, boost::python::converter::rvalue_from_python_stage1_data* data)
        {
            namespace py = boost::python;
            native_type* storage = new(reinterpret_cast<py::converter::rvalue_from_python_storage<native_type>*>(data)->storage.bytes) native_type();
            for (py::ssize_t i = 0, l = PySequence_Size(pyo); i < l; ++i) {
                storage->push_back(
                    py::extract<typename boost::range_value<native_type>::type>(
                        PySequence_GetItem(pyo, i)));
            }
            data->convertible = storage;
        }
    };
}

BOOST_PYTHON_MODULE(libclass4)
{
    using namespace boost::python;

    using s_add = void(foo::*)(int);
    using c_add = void(foo::*)(const foo::int_vector&);
    class_<foo>("foo")
        .def(
            "get_list",
            &foo::get_list,
            
            // 返り値のポリシー（C++クラスの内部で保持しているオブジェクトの参照を返す）
//            return_internal_reference<>()
            return_value_policy<copy_const_reference>()
        )
        .def("add", static_cast<s_add>(&foo::add))
        .def("add", static_cast<c_add>(&foo::add))
    ;
    
    // C++ -> Python
    to_python_converter<foo::int_vector, vector_to_pylist_converter<foo::int_vector>>();
    
    converter::registry::push_back(
        &pylist_to_vector_converter<foo::int_vector>::convertible,
        &pylist_to_vector_converter<foo::int_vector>::construct,
        boost::python::type_id<foo::int_vector>());

}
