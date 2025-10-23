#pragma once

#include <iostream>
#include <map>
#include <list>
#include <algorithm>
#include <vector>
#include <queue>
#include <tuple>
#include <limits>
#include <functional>
#include <set>
#include <numeric>

using namespace std;

template<typename VertexType>
struct DSU {
    vector<int> parent, rank;
    DSU(int n) : parent(n), rank(n, 0) {
        for (int i = 0; i < n; i++)
            parent[i] = i;
    }
    int find_set(int v) {
        if (v == parent[v]) return v;
        return parent[v] = find_set(parent[v]);
    }
    bool union_sets(int a, int b) {
        a = find_set(a);
        b = find_set(b);
        if (a != b) {
            if (rank[a] < rank[b]) swap(a, b);
            parent[b] = a;
            if (rank[a] == rank[b]) rank[a]++;
            return true;
        }
        return false;
    }
};

template<typename VertexType>
class Graph {
    map<VertexType, list<pair<VertexType, int>>> adjList;
    bool directed;

public:
    Graph(bool isDirected = false);

    void add_vertex(VertexType v);
    void remove_vertex(VertexType v);

    void add_edge(VertexType u, VertexType v, int weight = 1);
    void remove_edge(VertexType u, VertexType v);

    void print();

    const map<VertexType, list<pair<VertexType, int>>>& getAdjacency() const;

    // Minimum spanning tree (MST) algorithms
    pair<vector<pair<VertexType, VertexType>>, int> mst_prim(bool print);
    pair<vector<pair<VertexType, VertexType>>, int> mst_kruskal(bool print);
    pair<vector<pair<VertexType, VertexType>>, int> mst_boruvka(bool print);


    // Shortest path (Dijkstra)
    pair<vector<VertexType>, int> shortest_path(VertexType start, VertexType end, bool print);
};

#include "Graph.inl"
