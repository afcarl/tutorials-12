//
//  NautyDynamicGraphAlloc.h
//  static_allocation
//
//  Created by Seiya KUMADA on 2017/02/24.
//  Copyright (c) 2017年 熊田聖也. All rights reserved.
//

#ifndef static_allocation_NautyDynamicGraphAlloc_h
#define static_allocation_NautyDynamicGraphAlloc_h

#include <nauty.h>
#include <cstddef>

template<typename T>
class NautyDynamicGraphAlloc
{
private:
    T*&             name_;
    std::size_t&    name_sz_;
    
public:
    NautyDynamicGraphAlloc(T*& name, std::size_t& name_sz, int sz1, int sz2)
        : name_{name}
        , name_sz_{name_sz}
    {
        DYNALLOC2(T, name, name_sz, sz1, sz2, "malloc");
    }
    
    ~NautyDynamicGraphAlloc()
    {
        DYNFREE(name_, name_sz_);
    }
    
    NautyDynamicGraphAlloc& operator=(const NautyDynamicGraphAlloc&) = delete;
    NautyDynamicGraphAlloc(const NautyDynamicGraphAlloc&) = delete;
};

#endif
