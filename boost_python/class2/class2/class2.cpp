#include "class2.h"
#include <boost/python.hpp>

const foo::int_vector& foo::get_list() const {
    return v_;
}

void foo::add(int v) {
    v_.push_back(v);
}

BOOST_PYTHON_MODULE(libclass2)
{
    using namespace boost::python;

    class_<foo>("foo")
        .def(
            "get_list",
            &foo::get_list,
            
            // 返り値のポリシー（C++クラスの内部で保持しているオブジェクトを返す）
            return_internal_reference<>()
        )
        .def("add", &foo::add)
    ;
    
    // 返り値 foo::int_vectorをラップする。
    class_<foo::int_vector>("int_vector")
        .def(
            // 要素の取り出し
            "__getitem__",
            
            // (int const&(foo::int_vector::*)(foo::int_vector::size_type) const)は一体何？
            (int const&(foo::int_vector::*)(foo::int_vector::size_type) const)&foo::int_vector::at,

            // 返り値のポリシー（const参照値をコピーして返す。つまり値にして返す）
            return_value_policy<copy_const_reference>()
        )

    
        .def(
            // 長さ
            "__len__",
            &foo::int_vector::size
        )
    ;

}


