#!/usr/bin/env python2.7
# -*- coding: utf-8 -*-

import graph_tool
import graph_tool.draw
from numpy.random import randint, random
from itertools import izip

if __name__ == "__main__":
    # g = graph_tool.Graph(directed=True)

    # v1 = g.add_vertex()
    # v2 = g.add_vertex()
    # v3 = g.add_vertex()
    # v4 = g.add_vertex()
    # v5 = g.add_vertex()

    # e1 = g.add_edge(v1, v2)
    # e2 = g.add_edge(v1, v3)
    # e3 = g.add_edge(v1, v4)
    # e4 = g.add_edge(v2, v5)

    # graph_tool.draw.graph_draw(
    #     g,
    #     vertex_text=g.vertex_index,
    #     vertex_font_size=18,
    #     output_size=(200, 200),
    #     output="two-nodes.png")

    # for e in v1.out_edges():
    #     s = e.source()
    #     t = e.target()
    #     s_index = g.vertex_index[s]
    #     t_index = g.vertex_index[t]
    #     print("{} -> {}".format(s_index, t_index))

    # for v in v1.out_neighbours():
    #     index = g.vertex_index[v]
    #     print("{}".format(index))

    g = graph_tool.Graph()
    g.add_vertex(100)

    for (s, t) in izip(randint(0, 100, 100), randint(0, 100, 100)):
        g.add_edge(g.vertex(s), g.vertex(t))

    vprop_double = g.new_vertex_property("double")
    vprop_double[g.vertex(10)] = 3.1416

    vprop_vint = g.new_vertex_property("vector<int>")
    vprop_vint[g.vertex(40)] = [1, 3, 42, 54]

    eprop_dict = g.new_edge_property("object")
    eprop_dict[g.edges().next()] = {"foo": "bar", "gnu": 42}

    gprop_bool = g.new_graph_property("bool")
    gprop_bool[g] = True

    print(vprop_double[g.vertex(10)])
    print(vprop_vint[g.vertex(40)])
    print(g.num_vertices())
    vprop_double.get_array()[:] = random(g.num_vertices())
    # print(vprop_double.get_array())

    g.vertex_properties["score"] = vprop_double
    print(g.list_properties())

    gprop = g.new_graph_property("int")
    g.graph_properties["foo"] = gprop 
    g.graph_properties["foo"] = 42 

    print(g.graph_properties["foo"])

    v = g.vertex(0)
    print(g.vp["score"][v])
    print(g.vp.score[v])

    # from Graph I/O
