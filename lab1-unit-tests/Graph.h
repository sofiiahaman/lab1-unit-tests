/**
 * @file Graph.h
 * @author Sofiia Haman
 * @date 18.11.2025
 * @version 1.1
 * @brief Template-based weighted graph with MST and shortest-path algorithms.
 *
 * @details Provides a general-purpose directed or undirected graph
 * implemented via adjacency lists. Supports adding/removing vertices
 * and edges, computing MSTs (Prim, Kruskal, Boruvka), and shortest
 * paths via Dijkstra’s algorithm.
 */

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

/**
 * @struct DSU
 * @brief Disjoint Set Union (Union-Find) structure used for Kruskal's and Boruvka's algorithms.
 *
 * @tparam VertexType Type of vertices used for indexing (must be convertible to int indices externally).
 *
 * @details
 * The DSU supports two core operations:
 * - `find_set(v)` — finds the representative of the set containing vertex `v` (with path compression).
 * - `union_sets(a, b)` — merges the sets of two vertices.
 *
 * The structure is optimized with union by rank.
 */
template<typename VertexType>
struct DSU {
    vector<int> parent;  ///< Parent pointer array.
    vector<int> rank;   ///< Rank array for union by rank optimization.

    /**
    * @brief Constructs a DSU of given size.
    * @param n Number of elements in the union-find structure.
    */
    DSU(int n) : parent(n), rank(n, 0) {
        for (int i = 0; i < n; i++)
            parent[i] = i;
    }

    /**
     * @brief Finds the representative (root) of the set containing vertex @p v.
     * @param v Vertex index.
     * @return Root of the set.
     */
    int find_set(int v) {
        if (v == parent[v]) return v;
        return parent[v] = find_set(parent[v]);
    }

    /**
     * @brief Unites the sets containing @p a and @p b.
     * @param a First vertex.
     * @param b Second vertex.
     * @return True if union was successful, false if both vertices were already in the same set.
     */
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

/**
 * @class Graph
 * @brief General-purpose weighted graph supporting spanning tree and shortest path algorithms.
 *
 * @tparam VertexType Type used to identify vertices (e.g. string, int, custom struct with < operator).
 *
 * @details
 * The graph uses adjacency lists stored in an ordered `map`. Each vertex maps to a
 * list of pairs `(neighbor, weight)`. The graph may be directed or undirected.
 *
 * The class includes algorithms for:
 * - Minimum Spanning Tree: Prim, Kruskal, Boruvka
 * - Shortest Path: Dijkstra
 *
 * @note Requires VertexType to be comparable with `<` since it is a map key.
 *
 * @example
 * @code
 * Graph<int> g(false);
 * g.add_vertex(1);
 * g.add_vertex(2);
 * g.add_edge(1, 2, 10);
 * auto [mst_edges, total] = g.mst_prim(true);
 * auto [path, cost] = g.shortest_path(1, 2, true);
 * @endcode
 */
template<typename VertexType>
class Graph {
    map<VertexType, list<pair<VertexType, int>>> adjList;  ///< Adjacency list representation.
    bool directed;  ///< Flag indicating whether the graph is directed.

public:
    /**
     * @brief Constructs an empty graph.
     * @param isDirected If true, the graph will be directed.
     */
    Graph(bool isDirected = false);

    /**
     * @brief Adds a new vertex to the graph.
     * @param v Vertex identifier.
     */
    void add_vertex(VertexType v);

    /**
     * @brief Removes a vertex and all incident edges.
     * @param v Vertex identifier to remove.
     */
    void remove_vertex(VertexType v);

    /**
     * @brief Adds an edge between two vertices.
     *
     * @param u Source vertex.
     * @param v Destination vertex.
     * @param weight Weight of the edge (default 1).
     *
     * @note If the graph is undirected, a reverse edge is also added.
     */
    void add_edge(VertexType u, VertexType v, int weight = 1);

    /**
     * @brief Removes an edge between two vertices.
     * @param u Source vertex.
     * @param v Destination vertex.
     */
    void remove_edge(VertexType u, VertexType v);

    /**
     * @brief Prints the adjacency list of the graph.
     */
    void print();

    /**
     * @brief Returns a constant reference to the internal adjacency list.
     * @return Map of vertices to adjacency lists.
     */
    const map<VertexType, list<pair<VertexType, int>>>& getAdjacency() const;

    /**
     * @brief Computes a Minimum Spanning Tree using Prim’s algorithm.
     *
     * @param print If true, prints the MST during computation.
     * @return Pair of:
     *   - vector of edges (u, v),
     *   - total weight of the MST.
     */
    pair<vector<pair<VertexType, VertexType>>, int> mst_prim(bool print);

    /**
     * @brief Computes a Minimum Spanning Tree using Kruskal’s algorithm.
     *
     * @param print If true, prints details of the algorithm's steps.
     * @return Pair of:
     *   - vector of MST edges,
     *   - total weight of the MST.
     */
    pair<vector<pair<VertexType, VertexType>>, int> mst_kruskal(bool print);

    /**
     * @brief Computes a Minimum Spanning Tree using Boruvka’s algorithm.
     *
     * @param print If true, prints algorithm progress.
     * @return Pair of:
     *   - vector of MST edges,
     *   - total weight.
     */
    pair<vector<pair<VertexType, VertexType>>, int> mst_boruvka(bool print);

    /**
     * @brief Finds the shortest path between two vertices using Dijkstra's algorithm.
     *
     * @param start Start vertex.
     * @param end Destination vertex.
     * @param print If true, prints algorithm progress and results.
     *
     * @return Pair of:
     *   - vector of vertices representing the path,
     *   - total cost of the path.
     */
    pair<vector<VertexType>, int> shortest_path(VertexType start, VertexType end, bool print);
};

#include "Graph.inl"
