//
//  main.cpp
//  static_allocation
//
//  Created by 熊田聖也 on 2017/02/20.
//  Copyright © 2017年 熊田聖也. All rights reserved.
//

#include <iostream>
// If MAXN is defined as a positive integer, but not all objects are statically allocated.
#define MAXN 1000
#include "nauty.h"
#include <boost/format.hpp>

namespace
{
    DEFAULTOPTIONS_GRAPH(options);
}

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << MAXN << std::endl;
    
    // we change the option that we want to be different from the default.
    // the following line causes automorphisims to be written.
    options.writeautoms = true;
    
    graph g[MAXN * MAXN];
    
    while (1)
    {
        std::cout << "\nenter n: ";
        int n;
        std::cin >> n;
        if (n <= 0)
        {
            break;
        }
        
        if (n > MAXN)
        {
            std::cout << boost::format("n must be in the range 1..%1%\n") %MAXN;
            std::exit(1);
        }
        
        /*
         The nauty parameter m is a value such that an array of m setwords is sufficient to hold n bits.
         The type setword is defined in nauty.h
         The number of bits in a setword is WORDSIZE, which is 16, 32, or 64.
         Here we calculate m = ceiling(n/WORDSIZE)
         */
        
        const auto m = SETWORDSNEEDED(n);
        
        /*
         The following optional call verifies 
         that we are linking to compatible versions of the nauty routines.
         */
        // what does it mean?
        nauty_check(WORDSIZE, m, n, NAUTYVERSIONID);
        
        /*
         Now we create the cycle. First we zero the graph, then for each v,
         we add the edge (v,v+1), where values are mod n.
         */
        
        EMPTYGRAPH(g, m, n);
        for (auto v = 0; v < n; ++v )
        {
            ADDONEEDGE(g, v, (v + 1) % n, m);
        }
        
        std::cout << boost::format("Generators for Aut(C[%1%]:\n") %n;
        // what does Aut mean?
        
        /*
         Since we are not requiring a canonical labelling, the last
         parameter to densenauty() is not required and can be NULL.
         */
        
        int lab[MAXN];
        int ptr[MAXN];
        int orbits[MAXN];
        statsblk stats;
        densenauty(g, lab, ptr, orbits, &options, &stats, m, n, nullptr);
        
        
    }
    return 0;
}
