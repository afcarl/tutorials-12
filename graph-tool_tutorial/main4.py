#!/usr/bin/env python2.7
# -*- coding: utf-8 -*-

import graph_tool as gt
import numpy as np
import graph_tool.generation
import graph_tool.draw
import graph_tool.topology
import graph_tool.centrality

if __name__ == "__main__":

    g, pos = gt.generation.triangulation(
        np.random.random((500, 2)) * 4,
        type="delaunay")

    gt.draw.graph_draw(
        g,
        pos,
        output="raw.png")

    tree = gt.topology.min_spanning_tree(g)

    gt.draw.graph_draw(
        g,
        pos,
        edge_color=tree,
        output="min_tree.png")

    g.set_edge_filter(tree)
    gt.draw.graph_draw(g, pos, output="min_tree_filtered.png")

    bv, be = gt.centrality.betweenness(g)
    be.a /= (be.a.max() / 5)
    gt.draw.graph_draw(g, pos=pos, vertex_fill_color=bv, edge_pen_width=be, output="filtered-bt.png")

    g.set_edge_filter(None)
    bv, be = gt.centrality.betweenness(g)
    be.a /= (be.a.max() / 5)
    gt.draw.graph_draw(g, pos=pos, vertex_fill_color=bv, edge_pen_width=be, output="nonfiltered-bt.png")

    assert not g.is_directed(), ""

    ug = gt.GraphView(g, directed=True)
    assert ug.is_directed(), ""

    tv = gt.GraphView(g, efilt=tree)

    bv, be = gt.centrality.betweenness(tv)
    be.a /= (be.a.max() / 5)
    gt.draw.graph_draw(tv, pos=pos, vertex_fill_color=bv, edge_pen_width=be, output="mst-view.png")

    bv, be = gt.centrality.betweenness(g)
    u = gt.GraphView(g, efilt=lambda e: be[e] > be.a.max() / 2)
    be.a /= (be.a.max() / 5)
    gt.draw.graph_draw(u, pos=pos, vertex_fill_color=bv, output="central-edges-view.png")

    u = gt.GraphView(g, vfilt=lambda v: bv[v] > bv.a.max() / 2)
    gt.draw.graph_draw(u, pos=pos, vertex_fill_color=bv, output="central-vertices-view.png")
