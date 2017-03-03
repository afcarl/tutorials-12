#include "class3.h"
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

const foo::int_vector& foo::get_list() const {
    return v_;
}

void foo::add(int v) {
    v_.push_back(v);
}

BOOST_PYTHON_MODULE(libclass3)
{
    using namespace boost::python;

    class_<foo>("foo")
        .def(
            "get_list",
            &foo::get_list,
            
            // 返り値のポリシー（C++クラスの内部で保持しているオブジェクトの参照を返す）
            return_internal_reference<>()
        )
        .def("add", &foo::add)
    ;
    
    // 返り値 foo::int_vectorをラップする。
    class_<foo::int_vector>("int_vector")
        .def(vector_indexing_suite<foo::int_vector>())
    ;
}
