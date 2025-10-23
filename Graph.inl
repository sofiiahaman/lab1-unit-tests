#include "Graph.h"

template<typename VertexType>
Graph<VertexType>::Graph(bool isDirected) : directed(isDirected) {}

template<typename VertexType>
void Graph<VertexType>::add_vertex(VertexType v) {
    adjList.try_emplace(v);
}

template<typename VertexType>
void Graph<VertexType>::remove_vertex(VertexType v) {
    adjList.erase(v);
    for (auto& [_, neighbors] : adjList)
        neighbors.remove_if([v](auto const& edge) { return edge.first == v; });
}

template<typename VertexType>
void Graph<VertexType>::add_edge(VertexType u, VertexType v, int weight) {
    add_vertex(u);
    add_vertex(v);

    adjList[u].push_back({ v, weight });

    if (!directed && u != v)
        adjList[v].push_back({ u, weight });
}

template<typename VertexType>
void Graph<VertexType>::remove_edge(VertexType u, VertexType v) {
    if (adjList.find(u) != adjList.end()) {
        auto& lst = adjList[u];
        lst.remove_if([v](auto const& edge) { return edge.first == v; });
    }

    if (!directed && u != v && adjList.find(v) != adjList.end()) {
        auto& lst = adjList[v];
        lst.remove_if([u](auto const& edge) { return edge.first == u; });
    }
}

template<typename VertexType>
const map<VertexType, list<pair<VertexType, int>>>& Graph<VertexType>::getAdjacency() const {
    return adjList;
}

template<typename VertexType>
void Graph<VertexType>::print() {
    for (auto const& [vertex, neighbors] : adjList) {
        cout << vertex << " -> ";
        for (auto const& [to, w] : neighbors)
            cout << "(" << to << ", " << w << ") ";
        cout << endl;
    }
}

template<typename VertexType>
pair<vector<pair<VertexType, VertexType>>, int> Graph<VertexType>::mst_prim(bool print) {
    vector<pair<VertexType, VertexType>> mstEdges;
    int totalWeight = 0;

    if (adjList.empty()) {
        if (print) cout << "Graph is empty.\n";
        return { mstEdges, 0 };
    }
    if (directed) {
        if (print) cout << "Prim's algorithm works only for undirected graphs.\n";
        return { mstEdges, 0 };
    }

    VertexType start = adjList.begin()->first;
    map<VertexType, bool> inMST;
    for (auto const& [v, _] : adjList)
        inMST[v] = false;

    using Edge = pair<int, pair<VertexType, VertexType>>;
    priority_queue<Edge, vector<Edge>, greater<Edge>> pq;

    inMST[start] = true;
    for (auto const& [v, w] : adjList.at(start))
        pq.push({ w, {start, v} });

    while (!pq.empty()) {
        auto [weight, uv] = pq.top();
        pq.pop();
        VertexType u = uv.first, v = uv.second;

        if (inMST[v]) continue;

        inMST[v] = true;
        totalWeight += weight;
        mstEdges.push_back({ u, v });

        for (auto const& [to, w] : adjList.at(v))
            if (!inMST[to])
                pq.push({ w, {v, to} });
    }

    if (print) {
        cout << "Prim MST edges:\n";
        for (auto const& [u, v] : mstEdges)
            cout << u << " - " << v << "\n";
        cout << "Total weight = " << totalWeight << "\n";
    }

    return { mstEdges, totalWeight };
}

template<typename VertexType>
pair<vector<pair<VertexType, VertexType>>, int> Graph<VertexType>::mst_kruskal(bool print) {
    vector<pair<VertexType, VertexType>> mstEdges;
    int totalWeight = 0;

    if (adjList.empty()) {
        if (print) cout << "Graph is empty.\n";
        return { mstEdges, 0 };
    }
    if (directed) {
        if (print) cout << "Kruskal's algorithm works only for undirected graphs.\n";
        return { mstEdges, 0 };
    }

    vector<tuple<int, VertexType, VertexType>> edges;
    set<pair<VertexType, VertexType>> usedEdges;
    for (const auto& [u, neighbors] : adjList) {
        for (const auto& [v, w] : neighbors) {
            if (u != v && usedEdges.find({ v, u }) == usedEdges.end()) {
                edges.emplace_back(w, u, v);
                usedEdges.insert({ u, v });
            }
        }
    }

    sort(edges.begin(), edges.end(),
        [](auto const& a, auto const& b) { return get<0>(a) < get<0>(b); });

    map<VertexType, int> vertexToIndex;
    int idx = 0;
    for (auto const& [v, _] : adjList)
        vertexToIndex[v] = idx++;

    DSU<VertexType> dsu(idx);

    for (auto& [w, u, v] : edges) {
        int setU = dsu.find_set(vertexToIndex[u]);
        int setV = dsu.find_set(vertexToIndex[v]);
        if (setU != setV) {
            dsu.union_sets(setU, setV);
            mstEdges.push_back({ u, v });
            totalWeight += w;
        }
    }

    if (print) {
        cout << "Kruskal MST edges:\n";
        for (auto const& [u, v] : mstEdges)
            cout << u << " - " << v << "\n";
        cout << "Total weight = " << totalWeight << "\n";
    }

    return { mstEdges, totalWeight };
}

template<typename VertexType>
pair<vector<pair<VertexType, VertexType>>, int> Graph<VertexType>::mst_boruvka(bool print) {
    vector<pair<VertexType, VertexType>> mstEdges;
    int totalWeight = 0;

    if (directed) {
        if (print) cout << "Boruvka's algorithm works only for undirected graphs.\n";
        return { mstEdges, 0 };
    }

    int V = static_cast<int>(adjList.size());
    if (V == 0) {
        if (print) cout << "Graph is empty.\n";
        return { mstEdges, 0 };
    }

    vector<tuple<int, VertexType, VertexType>> edges;
    set<pair<VertexType, VertexType>> usedEdges;
    for (const auto& [u, neighbors] : adjList) {
        for (const auto& [v, w] : neighbors) {
            if (u != v && usedEdges.find({ v, u }) == usedEdges.end()) {
                edges.emplace_back(w, u, v);
                usedEdges.insert({ u, v });
            }
        }
    }

    map<VertexType, int> vertexToIndex;
    int idx = 0;
    for (auto const& [v, _] : adjList)
        vertexToIndex[v] = idx++;

    DSU<VertexType> dsu(idx);

    int numTrees = V;

    while (numTrees > 1) {
        vector<int> cheapest(idx, -1);

        for (int i = 0; i < static_cast<int>(edges.size()); i++) {
            auto [w, u, v] = edges[i];
            int set1 = dsu.find_set(vertexToIndex[u]);
            int set2 = dsu.find_set(vertexToIndex[v]);
            if (set1 == set2) continue;

            if (cheapest[set1] == -1 || get<0>(edges[cheapest[set1]]) > w)
                cheapest[set1] = i;
            if (cheapest[set2] == -1 || get<0>(edges[cheapest[set2]]) > w)
                cheapest[set2] = i;
        }

        bool anyUnion = false;
        for (int i = 0; i < idx; i++) {
            int edgeIndex = cheapest[i];
            if (edgeIndex != -1) {
                auto [w, u, v] = edges[edgeIndex];
                int set1 = dsu.find_set(vertexToIndex[u]);
                int set2 = dsu.find_set(vertexToIndex[v]);
                if (set1 == set2) continue;

                if (dsu.union_sets(set1, set2)) {
                    mstEdges.push_back({ u, v });
                    totalWeight += w;
                    numTrees--;
                    anyUnion = true;
                }
            }
        }

        if (!anyUnion) break;
    }

    if (print) {
        cout << "Boruvka MST edges:\n";
        for (auto const& [u, v] : mstEdges)
            cout << u << " - " << v << "\n";
        cout << "Total weight = " << totalWeight << "\n";
    }

    return { mstEdges, totalWeight };
}


template<typename VertexType>
pair<vector<VertexType>, int> Graph<VertexType>::shortest_path(VertexType start, VertexType end, bool print) {
    map<VertexType, double> dist;
    map<VertexType, VertexType> parent;

    for (auto const& [v, _] : adjList)
        dist[v] = numeric_limits<double>::infinity();

    dist[start] = 0;
    parent[start] = start;

    using P = pair<double, VertexType>;
    priority_queue<P, vector<P>, greater<P>> pq;
    pq.push({ 0, start });

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u]) continue;

        for (auto const& [v, w] : adjList[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                parent[v] = u;
                pq.push({ dist[v], v });
            }
        }
    }

    vector<VertexType> path;
    if (dist[end] == numeric_limits<double>::infinity()) {
        if (print)
            cout << "No path from " << start << " to " << end << endl;
        return { path, -1 };
    }

    for (VertexType v = end; v != start; v = parent[v])
        path.push_back(v);
    path.push_back(start);
    reverse(path.begin(), path.end());

    int totalDistance = static_cast<int>(dist[end]);

    if (print) {
        cout << "Shortest path: ";
        for (auto const& v : path)
            cout << v << " ";
        cout << "\nTotal distance: " << totalDistance << "\n";
    }

    return { path, totalDistance };
}