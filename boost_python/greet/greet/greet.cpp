/*
 *  boost_python_lib.cpp
 *  boost_python_lib
 *
 *  Created by 熊田聖也 on 2017/03/01.
 *  Copyright © 2017年 熊田聖也. All rights reserved.
 *
 */


#include "greet.hpp"
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>

const char* greet()
{
    return "hello, world";
}

BOOST_PYTHON_MODULE(libgreet/* ライブラリ名 */)
{
    using namespace boost::python;
    def(
        // pythonでの関数名
        "greet",
        
        // 対応するC++での関数名
        greet);
}
