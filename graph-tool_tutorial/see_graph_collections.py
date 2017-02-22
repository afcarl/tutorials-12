#!/usr/bin/env python2.7
# -*- coding: utf-8 -*-

import graph_tool.collection
import graph_tool.draw
import graph_tool.centrality


def see_graph(name, out_path):
    print("> {}".format(name))
    g = graph_tool.collection.data[name]
    print("graph is {}".format("directed" if g.is_directed() else "undirected"))
    print("the number of vertices:{}".format(g.num_vertices()))
    print("the number of edges:{}".format(g.num_edges()))
    bv, ev = graph_tool.centrality.betweenness(g)
    graph_tool.draw.graph_draw(
        g,
        pos=g.vp.pos,
        vertex_fill_color=bv,
        vertex_color=[0, 0, 0, 0],
        vertex_size=3,
        output=out_path)

    v = g.vertices().next()
    print(bv[v])
    print("vp")
    for (k, _) in g.vp.items():
        print(" - {}({},{})".format(k, g.vp[k][v], type(g.vp[k][v])))

    e = g.edges().next()
    print("ep")
    for (k, _) in g.ep.items():
        print(" - {}({},{})".format(k, g.ep[k][e], type(g.ep[k][e])))


def see_benchmark(name, input_path):
    print("> {}".format(name))
    g = graph_tool.load_graph(input_path)
    print(g.num_vertices())


if __name__ == "__main__":
    see_benchmark("mutag", "/Volumes/Untitled/mac/Data/patchy-san/pdb2graph/pdb/pdb1hga.ent")
    # see_graph("power", "power.png")
    # see_graph("polbooks", "polbooks.png")
    # see_graph("astro-ph", "astro-ph.png")
    # see_graph("email-enron", "email-enron.png")
