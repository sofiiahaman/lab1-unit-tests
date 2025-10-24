#include "pch.h"
#include "Graph.h"


TEST(GraphTest, AddAndRemoveVerticesAndEdges) {
    Graph<int> g(false);

    g.add_vertex(1);
    g.add_vertex(2);
    g.add_edge(1, 2, 5);

    auto adj = g.getAdjacency();
    ASSERT_EQ(adj.size(), 2);
    EXPECT_EQ(adj.at(1).front().first, 2);
    EXPECT_EQ(adj.at(1).front().second, 5);

    g.remove_edge(1, 2);
    EXPECT_TRUE(g.getAdjacency().at(1).empty());

    g.remove_vertex(1);
    EXPECT_EQ(g.getAdjacency().count(1), 0);
}

TEST(GraphTest, MSTPrim) {
    Graph<int> g(false);
    g.add_edge(1, 2, 2);
    g.add_edge(1, 3, 3);
    g.add_edge(2, 3, 1);
    g.add_edge(2, 4, 4);
    g.add_edge(3, 4, 5);

    auto [edges, weight] = g.mst_prim(false);

    EXPECT_EQ(weight, 7);
    EXPECT_EQ(edges.size(), 3);

    for (auto [u, v] : edges) {
        EXPECT_GE(u, 1);
        EXPECT_LE(v, 4);
    }
}

TEST(GraphTest, MSTKruskal) {
    Graph<int> g(false);
    g.add_edge(10, 20, 5);
    g.add_edge(10, 30, 6);
    g.add_edge(20, 30, 2);
    g.add_edge(30, 40, 3);
    g.add_edge(20, 40, 7);

    auto [edges, weight] = g.mst_kruskal(false);

    EXPECT_EQ(edges.size(), 3);
    EXPECT_EQ(weight, 10);
}

TEST(GraphTest, MSTBoruvka) {
    Graph<int> g(false);
    g.add_edge(1, 2, 4);
    g.add_edge(1, 3, 3);
    g.add_edge(2, 3, 2);
    g.add_edge(3, 4, 1);
    g.add_edge(4, 5, 5);

    auto [edges, weight] = g.mst_boruvka(false);

    EXPECT_EQ(edges.size(), 4);
    EXPECT_EQ(weight, 11);
}

TEST(GraphTest, DijkstraShortestPath) {
    Graph<int> g(true);
    g.add_edge(1, 2, 2);
    g.add_edge(2, 3, 3);
    g.add_edge(1, 3, 10);
    g.add_edge(3, 4, 1);

    auto [path, dist] = g.shortest_path(1, 4, false);

    EXPECT_EQ(dist, 6);
    vector<int> expected = { 1, 2, 3, 4 };
    EXPECT_EQ(path, expected);
}

TEST(GraphTest, DijkstraNoPath) {
    Graph<int> g(true);
    g.add_edge(1, 2, 5);
    g.add_vertex(3);

    auto [path, dist] = g.shortest_path(1, 3, false);
    EXPECT_EQ(dist, -1);
    EXPECT_TRUE(path.empty());
}

TEST(GraphTest, EmptyGraphMST) {
    Graph<int> g(false);

    auto [edges1, weight1] = g.mst_prim(false);
    EXPECT_TRUE(edges1.empty());
    EXPECT_EQ(weight1, 0);

    auto [edges2, weight2] = g.mst_kruskal(false);
    EXPECT_TRUE(edges2.empty());
    EXPECT_EQ(weight2, 0);
}

TEST(GraphTest, DirectedGraphMSTNotAllowed) {
    Graph<int> g(true);
    g.add_edge(1, 2, 10);

    auto [edgesPrim, weightPrim] = g.mst_prim(false);
    EXPECT_TRUE(edgesPrim.empty());
    EXPECT_EQ(weightPrim, 0);

    auto [edgesKruskal, weightKruskal] = g.mst_kruskal(false);
    EXPECT_TRUE(edgesKruskal.empty());
    EXPECT_EQ(weightKruskal, 0);
}

TEST(GraphTest, LoopEdgeIgnored) {
    Graph<int> g(false);

    g.add_edge(1, 1, 10);
    g.add_edge(1, 2, 1);
    g.add_edge(2, 3, 2);

    auto [edges, weight] = g.mst_prim(false);
    EXPECT_EQ(weight, 3);
    EXPECT_EQ(edges.size(), 2);

    std::set<std::pair<int, int>> expectedEdges = { {1,2}, {2,3} };
    std::set<std::pair<int, int>> actualEdges;
    for (auto const& e : edges) {
        actualEdges.insert(e);
    }
    EXPECT_EQ(actualEdges, expectedEdges);
}