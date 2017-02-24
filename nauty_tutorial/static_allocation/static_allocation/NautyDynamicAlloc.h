//
//  NautyDynamicAlloc.h
//  static_allocation
//
//  Created by Seiya KUMADA on 2017/02/24.
//  Copyright (c) 2017年 熊田聖也. All rights reserved.
//

#ifndef __static_allocation__NautyDynamicAlloc__
#define __static_allocation__NautyDynamicAlloc__


#include <nauty.h>
#include <cstddef>

template<typename T>
class NautyDynamicAlloc
{
private:
    T*&              name_;
    std::size_t&    name_sz_;
    
public:
    NautyDynamicAlloc(T*& name, std::size_t& name_sz, int n)
        : name_{name}
        , name_sz_{name_sz}
    {
        DYNALLOC1(T, name, name_sz, n, "malloc");
    }
    
    ~NautyDynamicAlloc()
    {
        DYNFREE(name_, name_sz_);
    }
    
    NautyDynamicAlloc& operator=(const NautyDynamicAlloc&) = delete;
    NautyDynamicAlloc(const NautyDynamicAlloc&) = delete;
};

#endif /* defined(__static_allocation__NautyDynamicAlloc__) */
