#!/usr/bin/env python2.7
# -*- coding: utf-8 -*-

import graph_tool as gt
import graph_tool.stats
import graph_tool.draw
import numpy as np
import pylab
import matplotlib

if __name__ == "__main__":
    # We start with an empty, directed graph.
    g = gt.Graph()

    # We want also to keep the age information for each vertex and edge.
    # For that let's create some property maps.
    v_age = g.new_vertex_property("int")
    e_age = g.new_edge_property("int")

    # The final size of the network.
    N = 100000

    # We have to start with one vertex.
    v = g.add_vertex()
    v_age[v] = 0

    # We will keep a list of the vertices. The number of times a vertex is in this list
    # will give the probability of it being selected.
    vlist = [v]

    # let's now add the new edges and vertices
    for i in range(1, N):
        # create our new vertex
        v = g.add_vertex()
        v_age[v] = i

        # we need to sample a new vertex to be the target, based on its in-degree+1.
        # For that, we simply randomly sample it from vlist.
        i = np.random.randint(0, len(vlist))
        target = vlist[i]

        # add edge
        e = g.add_edge(v, target)
        e_age[e] = i

        # put v and target in the list
        vlist.append(target)
        vlist.append(v)

    # now we have a graph!

    # let's do a random walk on the graph and print the age of the vertices we find,
    # just for fun.
    v = g.vertex(np.random.randint(0, g.num_vertices()))
    while True:
        print("vertex:{}, in-degree:{}, out-degree:{}, age:{}".format(int(v), v.in_degree(), v.out_degree(), v_age[v]))

        if v.out_degree() == 0:
            print("Nowhere else to go... We found the main hub!")
            break

        n_list = []
        for w in v.out_neighbours():
            n_list.append(w)
        v = n_list[np.random.randint(0, len(n_list))]

    # let's save our graph for posterity. We want to save the age properties as well...
    # To do this, they must become "internal" properties:

    g.vp["age"] = v_age
    g.ep["age"] = e_age

    # now we can save it
    g.save("price.xml.gz")

    # let's plot its in-degree distribution
    in_hist = gt.stats.vertex_hist(g, "in")

    y = in_hist[0]
    x = in_hist[1]
    pylab.figure(figsize=(6, 4))
    pylab.hist(y, x)
    pylab.gca().set_yscale("log")
    pylab.gca().set_ylim(1e-1, 1e5)

    pylab.subplots_adjust(left=0.2, bottom=0.2)
    pylab.xlabel("$k_{in}$")
    pylab.ylabel("$NP(k_{in})$")
    pylab.tight_layout()
    pylab.savefig("price-in-deg-dist.png")

    # let's plot its in-degree distribution
#    out_hist = gt.stats.vertex_hist(g, "out")
#
#    y = out_hist[0]
#    x = out_hist[1]
#    pylab.figure(figsize=(6, 4))
#    pylab.hist(y, x)
#    pylab.gca().set_yscale("log")
#    pylab.gca().set_ylim(1e-1, 1e5)
#
#    pylab.subplots_adjust(left=0.2, bottom=0.2)
#    pylab.xlabel("$k_{out}$")
#    pylab.ylabel("$NP(k_{out})$")
#    pylab.tight_layout()
#    pylab.savefig("price-out-deg-dist.png")

    g = gt.load_graph("price.xml.gz")
    age = g.vp.age

    pos = graph_tool.draw.sfdp_layout(g)

    # It takes long long time to draw an image.
    gt.draw.graph_draw(
        g,
        pos,
        output_size=(1000, 1000),
        vertex_color=[1, 1, 1, 0],
        vertex_fill_color=age,
        vertex_size=1,
        edge_pen_width=1.2,
        vcmap=matplotlib.cm.gist_heat_r,
        output="price.png")
