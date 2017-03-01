//
//  greet.cpp
//  boost_python
//
//  Created by 熊田聖也 on 2017/03/01.
//  Copyright © 2017年 熊田聖也. All rights reserved.
//

#include "greet.hpp"
#include <boost/python.hpp>

const char* greet()
{
    return "hello, world";
}

