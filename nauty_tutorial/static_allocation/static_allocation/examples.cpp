//
//  examples.cpp
//  static_allocation
//
//  Created by Seiya KUMADA on 2017/02/23.
//  Copyright (c) 2017年 熊田聖也. All rights reserved.
//

#include "examples.h"

// If MAXN is defined as a positive integer, but not all objects are statically allocated.
#define MAXN 1000 // 許容する頂点の数。動的アロケータを使う場合は0にする。#include "nauty.h"の前に書く。
#include <nauty.h>
#include <traces.h>
#include <nausparse.h>
#include <iostream>
#include <boost/format.hpp>
#include <cpplinq.hpp>
#include <vector>
#include <boost/format.hpp>
#include <climits>
#include "NautyDynamicAlloc.h"
#include "NautyDynamicGraphAlloc.h"
#include <map>

namespace
{
    

    template<typename T, int N>
    inline void print_result(const std::string& name, T (&array)[N], int n)
    {
        std::cout << name;
        cpplinq::from_array(array) >>
            cpplinq::zip_with(cpplinq::range(0, n)) >>
            cpplinq::for_each(
                [](auto x){ std::cout << x.first << ","; }
            );
        std::cout << std::endl;
    }

    template<typename T>
    inline void print_result(const std::string& name, const T& beg, const T& end)
    {
        std::cout << name;
        cpplinq::from_iterators(beg, end) >>
            cpplinq::for_each(
                [](auto x)
                {
                    std::cout << x << ",";
                }
            );
        std::cout << std::endl;
    }

    template<typename T>
    inline void print_result(const std::string& name, const T& range)
    {
        print_result(name, std::begin(range), std::end(range));
    }

    template<typename T>
    inline void print_ptn(const std::string& name, const T& beg, const T& end)
    {
        std::cout << name;
        cpplinq::from_iterators(beg, end) >>
            cpplinq::for_each(
                [](auto x)
                {
                    std::cout << (x ? 1 : 0) << ",";
                }
            );
        std::cout << std::endl;
    }

    template<typename T>
    inline void print_ptn(const std::string& name, const T& range)
    {
        print_ptn(name, std::begin(range), std::end(range));
    }
}

/*
    This program prints generators for the automorphism group of an n-vertex polygon,
    where n is a number supplied by the user.
*/
void example_0()
{
    DEFAULTOPTIONS_GRAPH(options);
    
    // we change the option that we want to be different from the default.
    // the following line causes automorphisims to be written.
    options.writeautoms = true;
    
    // グラフはMAXNxMAXNのサイズを持つ行列で表現される。隣接関係の有無は1ビットで表現される。
    graph g[MAXN * MAXN];
    /*
     
    */
    
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
         ここでは setwordはunsigned long型である。
         */
        
        const auto m = SETWORDSNEEDED(n);
        std::cout << "m: " << m << std::endl;
        
        /*
         The following optional call verifies 
         that we are linking to compatible versions of the nauty routines.
         */
        std::cout << "what does it mean?" << std::endl;
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
        
        std::cout << boost::format("Generators for Aut(C[%1%]):\n") %n;
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
        
        print_result("lab: ", lab, n);
        print_result("ptr: ", ptr, n);
        print_result("orbits: ", orbits, n);
        
        
        /*
         The size of the group is returned in stats.grpsize1 and stats.grpsize2.
        */
        std::cout << "Automorphism group size( grpsize1 x 10^{grpsize2} ) = ";
        writegroupsize(stdout, stats.grpsize1, stats.grpsize2);
        std::cout << std::endl;
        std::cout << "tats.grpsize1: " << stats.grpsize1 << std::endl;
        std::cout << "tats.grpsize2: " << stats.grpsize2 << std::endl;
        //
    }
}

/*
    This program represents a sample described at p2-p3 in the tutorial book
*/
void example_1()
{
    DEFAULTOPTIONS_GRAPH(options);
    options.writeautoms = true;
    graph g[MAXN * MAXN];
    const auto n = 8;
    const auto m = SETWORDSNEEDED(n);
    
    EMPTYGRAPH(g, m, n);
    
    ADDONEEDGE(g, 0, 1, m);
    ADDONEEDGE(g, 1, 3, m);
    ADDONEEDGE(g, 3, 2, m);
    ADDONEEDGE(g, 2, 0, m);
    
    ADDONEEDGE(g, 2, 4, m);
    ADDONEEDGE(g, 3, 5, m);
    ADDONEEDGE(g, 2, 5, m);
    ADDONEEDGE(g, 3, 4, m);
    
    ADDONEEDGE(g, 4, 6, m);
    ADDONEEDGE(g, 6, 7, m);
    ADDONEEDGE(g, 7, 5, m);
    ADDONEEDGE(g, 5, 4, m);
    
    int lab[MAXN];
    int ptr[MAXN];
    int orbits[MAXN];
    statsblk stats;
    densenauty(g, lab, ptr, orbits, &options, &stats, m, n, nullptr);
    
    print_result("lab: ", lab, n);
    print_result("ptr: ", ptr, n);
    print_result("orbits: ", orbits, n);
    
    
    /*
     The size of the group is returned in stats.grpsize1 and stats.grpsize2.
    */
    std::cout << "Automorphism group size( grpsize1 x 10^{grpsize2} ) = ";
    writegroupsize(stdout, stats.grpsize1, stats.grpsize2);
    std::cout << std::endl;
    std::cout << "tats.grpsize1: " << stats.grpsize1 << std::endl;
    std::cout << "tats.grpsize2: " << stats.grpsize2 << std::endl;
    
}

/*
    This program prints generators for the automorphism group of an n-vertex polygon,
    where n is a number of supplied by the user.
    This version uses dynamic allocation.
*/
void example_2()
{
    DEFAULTOPTIONS_GRAPH(options);
    /*
        DYNALISTAT declares a pointer variable (to hold an array when it is allocated) 
        and a size variable to remember how big the array is.
        Nothing is allocated yet.
    */
    
    DYNALLSTAT(graph, g, g_sz);
    DYNALLSTAT(int, lab, lab_sz);
    DYNALLSTAT(int, ptr, ptr_sz);
    DYNALLSTAT(int, orbits, orbits_sz);
    
    /*
        Default options are set by the DEFAULTOPTIONS_GRAPH macro above.
        Here we change those options that we want to different from the
        defaults. writeautoms = TRUE causes automorphism to be written.
    */
    
    options.writeautoms = true;
    
    while (true)
    {
        std::cout << "\nenter n: ";
        int n;
        std::cin >> n;
        if (n > 0)
        {
            /*
                The nauty parameter m is a value such that an array of m setwords is sufficient to hold n bits.
                The type setword is defined in nauty.h. The number of bits in a setword is WORDSIZE, which is 16, 32, or 64
                Here we calculate m = ceiling(n/WORDSIZE).
            */
            
            const auto m = SETWORDSNEEDED(n);
            
            /*
                The following optional call verifies that we are linking to compatible versions of the nauty routines.
            */
            
            nauty_check(WORDSIZE, m, n, NAUTYVERSIONID);
            
            /*
                Now that we know how big the graph will be, we allocate space for the graph and the other arrays we need.
            */
            
            DYNALLOC2(graph, g,      g_sz,      m, n, "malloc");
            DYNALLOC1(int,   lab,    lab_sz,    n,    "malloc");
            DYNALLOC1(int,   ptr,    ptr_sz,    n,    "malloc");
            DYNALLOC1(int,   orbits, orbits_sz, n,    "malloc");
            
            EMPTYGRAPH(g, m, n);
            
            for (auto i = 0; i < n; ++i)
            {
                ADDONEEDGE(g, i, (i+1)%n, m);
            }
            
            std::cout << boost::format("Generators for Aut(C[%1%]):\n") %n;
            statsblk stats;
            densenauty(g, lab, ptr, orbits, &options, &stats, m, n, nullptr);
            
            /*
             The size of the group is returned in stats.grpsize1 and stats.grpsize2.
            */
            std::cout << "Automorphism group size( grpsize1 x 10^{grpsize2} ) = ";
            writegroupsize(stdout, stats.grpsize1, stats.grpsize2);
            std::cout << std::endl;
            std::cout << "tats.grpsize1: " << stats.grpsize1 << std::endl;
            std::cout << "tats.grpsize2: " << stats.grpsize2 << std::endl;

        }
        else
        {
        
        }
    }
    
}


template<typename T>
class Type;

/*
    This program demonstrates how an isomorphism is found between two graphs,
    using the Moebius graphs as an example.
    This version uses dense form with dynamic allocation.
*/
void example_3()
{
    
    DYNALLSTAT(int, lab1, lab1_sz);
    DYNALLSTAT(int, lab2, lab2_sz);
    DYNALLSTAT(int, ptn, ptn_sz);
    DYNALLSTAT(int, orbits, orbits_sz);
    DYNALLSTAT(graph, g1, g1_sz);
    DYNALLSTAT(graph, g2, g2_sz);
    DYNALLSTAT(graph, cg1, cg1_sz);
    DYNALLSTAT(graph, cg2, cg2_sz);
    
    DEFAULTOPTIONS_GRAPH(options);
    
    /*
        We want to find a canonical labelling.
     */
    options.getcanon = true;
    
    while (true)
    {
        std::cout << "\nenter n: ";
        int n;
        std::cin >> n;
        if (n > 0)
        {
            if (n % 2 != 0)
            {
                std::cerr << "Sorry, n must be even\n";
                continue;
            }
            
            const auto m = SETWORDSNEEDED(n);
            nauty_check(WORDSIZE, m, n, NAUTYVERSIONID);
            
            /*
                Now make the first graph
             */

            NautyDynamicGraphAlloc<graph> g1_alloc{g1, g1_sz, n, m};
            EMPTYGRAPH(g1, m, n);
            
            /*
                Spokes
             */
            for (auto i = 0; i < n; i += 2)
            {
                ADDONEEDGE(g1, i, i + 1, m);
            }
            
            /*
                Cycle
             */
            for (auto i = 0; i < n - 2; ++i )
            {
                ADDONEEDGE(g1, i, i + 2, m);
            }
            ADDONEEDGE(g1, 1, n - 2, m);
            ADDONEEDGE(g1, 0, n - 1, m);
            
            /*
                Now make the second graph
             */
            NautyDynamicGraphAlloc<graph> g2_alloc{g2, g2_sz, n, m};
            EMPTYGRAPH(g2, m, n);
            
            for (auto i = 0; i < n; ++i )
            {
                ADDONEEDGE(g2, i, (i + 1) % n, m);  // Rim
                ADDONEEDGE(g2, i, (i + n / 2) % n, m); // Diagonals
            }
            
            /*
                Create canonical graphs
             */
            
            NautyDynamicAlloc<int> lab1_alloc   {lab1, lab1_sz, n};
            NautyDynamicAlloc<int> lab2_alloc   {lab2, lab2_sz, n};
            NautyDynamicAlloc<int> ptn_alloc    {ptn, ptn_sz, n};
            NautyDynamicAlloc<int> orbits_alloc {orbits, orbits_sz, n};
           
            NautyDynamicGraphAlloc<graph> cg1_alloc{cg1, cg1_sz, n, m};
            NautyDynamicGraphAlloc<graph> cg2_alloc{cg2, cg2_sz, n, m};
            
            statsblk stats;
            densenauty(g1, lab1, ptn, orbits, &options, &stats, m, n, cg1);
            densenauty(g2, lab2, ptn, orbits, &options, &stats, m, n, cg2);
            
            /*
                Compare canonically labelled graphs
             */
            auto is_isomorphic = true;
            for (auto k = 0; k < m * n; ++k )
            {
                if (cg1[k] != cg2[k])
                {
                    is_isomorphic = false;
                    break;
                }
            }

            if (is_isomorphic)
            {
                std::cout << "Isomorphic\n";
                if (n <= 1000)
                {
                    /*
                        Write the isomorphism.
                        For each i, vertex lab1[i] of sg1 maps onto vertex lab2[i] of sg2.
                        We compute the map in order of labelling because it looks better.
                    */
                    
                    std::map<int, int> map{};
                    cpplinq::range(0, n) >> cpplinq::for_each([&map](auto i){
                        map[lab1[i]] = lab2[i];
                    });

                    if (n == 10)
                    {
                        auto answers = std::map<int, int>{
                            {0, 0}, {1, 5}, {2, 1}, {3, 6}, {4, 2}, {5, 7}, {6, 3}, {7, 8}, {8, 4}, {9, 9}
                        };
                    
                        cpplinq::from(map)
                            >> cpplinq::for_each([&answers](const auto& pair)
                                {
                                    assert(answers[pair.first] == pair.second);
                                });
                    }
                    cpplinq::from(map) >> cpplinq::for_each([](const auto& pair){
                        std::cout << boost::format(" %1%-%2%") %pair.first %pair.second;
                    });
                    std::cout << std::endl;
                    
                    
                }
            }
            else
            {
                std::cout << "Not isomorphic\n";
            }
        }
        else
        {
            break;
        }
    }
}

namespace
{
    void make_first_graph(graph* g1, int n, int m)
    {
        // Spokes
        for (auto i = 0; i < n; i += 2)
        {
            ADDONEEDGE(g1, i, i + 1, m);
        }
        
        // Cycle
        for (auto i = 0; i < n - 2; ++i )
        {
            ADDONEEDGE(g1, i, i + 2, m);
        }
        ADDONEEDGE(g1, 1, n - 2, m);
        ADDONEEDGE(g1, 0, n - 1, m);
    }
    
    void make_second_graph(graph* g2, int n, int m)
    {
        for (auto i = 0; i < n; ++i )
        {
            ADDONEEDGE(g2, i, (i + 1) % n, m);  // Rim
            ADDONEEDGE(g2, i, (i + n / 2) % n, m); // Diagonals
        }
    }
}

void example_4()
{
    DEFAULTOPTIONS_GRAPH(options);
    
    // The initial colouring of the graph is defined.
    options.getcanon = true;
    
    // the number of vertices
    const auto n = 10;
    
    // m is a value such that an array of m setwords is sufficient to hold n bits.
    const auto m = SETWORDSNEEDED(n);
    
    // The following optional call verifies that we are linking to compatible versions of the nauty routines.
    nauty_check(WORDSIZE, m, n, NAUTYVERSIONID);
    
    // Now make the first graph
    std::vector<graph> g1(n * m, 0);
    make_first_graph(g1.data(), n, m);

    // Now make the second graph
    std::vector<graph> g2(n * m, 0);
    make_second_graph(g2.data(), n, m);
    
    // Create canonical graphs

    std::vector<int> lab1(n);
    std::vector<int> lab2(n);
    std::vector<int> ptn1(n);
    std::vector<int> ptn2(n);
    std::vector<int> orbits1(n);
    std::vector<int> orbits2(n);
   
    std::vector<graph> cg1(n * m, 0);
    std::vector<graph> cg2(n * m, 0);
    
    statsblk stats;
    densenauty(g1.data(), lab1.data(), ptn1.data(), orbits1.data(), &options, &stats, m, n, cg1.data());
    densenauty(g2.data(), lab2.data(), ptn2.data(), orbits2.data(), &options, &stats, m, n, cg2.data());
    
    std::cout << "      ";
    cpplinq::range(0, n) >> cpplinq::for_each([](auto i){ std::cout << i << ","; });
    std::cout << std::endl;
    print_result("lab1: ", lab1);
    print_result("lab2: ", lab2);
    std::cout << std::endl;
    
    print_result("lab1: ", lab1);
    print_ptn("ptn1: ", ptn1);
    print_result("lab2: ", lab2);
    print_ptn("ptn2: ", ptn2);
    
    print_result("orbits1: ",  orbits1);
    print_result("orbits2: ",  orbits2);
    
    std::map<int, int> map{};
    cpplinq::range(0, n) >> cpplinq::for_each([&map, &lab1, &lab2](auto i){
        map[lab1[i]] = lab2[i];
    });

    auto answers = std::map<int, int>{
        {0, 0}, {1, 5}, {2, 1}, {3, 6}, {4, 2}, {5, 7}, {6, 3}, {7, 8}, {8, 4}, {9, 9}
    };
    
    auto is_isomorphic = true;
    for (auto k = 0; k < m * n; ++k )
    {
        if (cg1[k] != cg2[k])
        {
            is_isomorphic = false;
            break;
        }
    }
    
    assert(is_isomorphic);
    cpplinq::from(map) >>
        cpplinq::for_each(
            [&answers](const auto& pair)
            {
                assert(answers[pair.first] == pair.second);
            }
        );
}

void example_5()
{
    DEFAULTOPTIONS_GRAPH(options);
    
    // The initial colouring of the graph is defined.
    options.getcanon = true;
    options.defaultptn = false;
    options.writeautoms = true;

    // the number of vertices
    const auto n = 10;

    std::vector<int> lab1 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<int> ptn1 = {1, 1, 1, 0, 0, 0, 0, 0, 0, 0};
    std::vector<int> orbits1(n);
    
    
    // m is a value such that an array of m setwords is sufficient to hold n bits.
    const auto m = SETWORDSNEEDED(n);
    
    // The following optional call verifies that we are linking to compatible versions of the nauty routines.
    nauty_check(WORDSIZE, m, n, NAUTYVERSIONID);
    
    // Now make the first graph
    std::vector<graph> g1(n * m, 0);
    make_first_graph(g1.data(), n, m);
    
    
    // Create canonical graphs

   
    std::vector<graph> cg1(n * m, 0);

    print_result("before lab1: ", lab1);
    print_ptn("before ptn1: ", ptn1);
    
    statsblk stats;
    densenauty(g1.data(), lab1.data(), ptn1.data(), orbits1.data(), &options, &stats, m, n, cg1.data());
    
    print_result("after  lab1: ", lab1);
    print_ptn("after  ptn1: ", ptn1);
    
     writegroupsize(stdout, stats.grpsize1, stats.grpsize2);
     std::cout << std::endl;
}