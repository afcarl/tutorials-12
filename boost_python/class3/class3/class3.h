/*
 *  class3.h
 *  class3
 *
 *  Created by Seiya KUMADA on 2017/03/03.
 *  Copyright (c) 2017年 kumada. All rights reserved.
 *
 */

#ifndef FOO_H
#define FOO_H

#include <vector>

class foo {
public:
    typedef std::vector<int> int_vector;

public:
    const int_vector& get_list() const;
    void add(int v);

private:
    int_vector v_;
};

#endif // FOO_H