/*
 *  classPriv.h
 *  class
 *
 *  Created by Seiya KUMADA on 2017/03/02.
 *  Copyright (c) 2017年 kumada. All rights reserved.
 *
 */

#include <boost/python.hpp>

class accumulator {
public:
    int operator()(int v) {
        v_ += v;
        return v_;
    }

    int value() const {
        return v_;
    }

private:
    int v_;
};

BOOST_PYTHON_MODULE(libclass/* ライブラリ名 */)
{
    using namespace boost::python;

    class_<accumulator>("accumulator"/* pythonでのクラス名 */)
    
        .def(
            // pythonでの関数名
            "__call__",
            
            // 対応するC++での関数名
            &accumulator::operator())
    
        // accumulator::valueをpythonのpropertyとして公開する。
        .add_property(
            // pythonでのプロパティ名
            "value",
            
            // 対応するC++での関数名
            &accumulator::value)
        ;
}