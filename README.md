# Transport and Environment Simulation System
This program simulates a transport system within a geographical environment using graph-based algorithms.

## **Graph module:**
A template-based weighted graph that supports both directed and undirected graphs.

**Key Features:**

- *add_vertex(v) / add_edge(u, v, weight) / remove_vertex(v) / remove_edge(u, v)*

- Display adjacency list with print()

- Minimum Spanning Tree algorithms:
  - *mst_prim(print)*
  - *mst_kruskal(print)*
  - *mst_boruvka(print)*
  
- Shortest path:

*shortest_path(start, end, print)* (Dijkstra’s algorithm)

## **Transport module:**

Models different types of vehicles with fuel, speed, and movement behavior.

**Base Class** - Transport

**Core attributes:** name, speed, position

**Functions:** *move(distance), accelerate(increment), brake(decrement), info()*

**Derived Classes:** 
- LandTransport
- WaterTransport
- AirTransport 

**Specific Vehicle Classes:** 
- Car
- Train
- Yacht
- Helicopter

## **Environment module:**
Defines the geographical environment.

- *MapObject* - base class for all objects on the map (provides coordinates x, y).

- *Point* - represents a location on the map (e.g., a city, port, or station).

- *Obstacle* - represents barriers or challenges in the environment (e.g., mountains, storms, traffic jams).

- *Route* - defines a connection between two points with a specific distance.

- *Environment* - the main environment manager that:

  - Stores all routes and obstacles
  - Provides methods for displaying the environment
  - Interacts with the transport system for route planning and simulation

**Key Features:**

- *addRoute(route)*
- *addObstacle(obstacle)*
- *showEnvironment()* - displays routes and obstacles
- *findOptimalRoute(graph, start, end, transport)* - finds the best route using Dijkstra
- *moveTransport(transport, route)* - simulates transport movement
