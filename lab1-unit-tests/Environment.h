/**
 * @file Environment.h
 * @author Sofiia Haman
 * @date 18.11.2025
 * @version 1.0
 * @brief Declarations of map-related classes: MapObject, Point, Obstacle, Route, and Environment.
 *
 * This header defines the core spatial entities used in the navigation subsystem,
 * including map points, obstacles, routes, and the environment manager.
 *
 * @details
 * The module provides:
 * - an abstract base class MapObject representing any spatial entity on a 2D map,
 * - Point for named map locations,
 * - Obstacle for various environmental obstructions,
 * - Route for directed connections between points,
 * - Environment for managing all navigational objects and interacting
 *   with external graph and transport systems.
 *
 * These classes support route planning, movement simulation, and environment visualization.
 */

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Transport.h"
#include "Graph.h" 
using namespace std;

/**
 * @class MapObject
 * @brief Abstract base class representing any object that exists on a 2D map.
 *
 * Provides spatial coordinates (x, y) and requires derived objects
 * to implement getInfo().
 * 
 * @example
 * @code
 * MapObject* obj = new Point("City A", 10, 20);
 * cout << obj->getInfo();
 * delete obj;
 * @endcode
 */
class MapObject {
protected:
    double x;
    double y;

public:
    /**
    * @brief Constructs a MapObject at the specified coordinates.
    * @param xCoord X position.
    * @param yCoord Y position.
    */
    MapObject(double xCoord, double yCoord) : x(xCoord), y(yCoord) {}

    /// Virtual destructor for safe polymorphic cleanup.
    virtual ~MapObject() = default;

    /// @return X-coordinate of the map object.
    double getX() const { return x; }

    /// @return Y-coordinate of the map object.
    double getY() const { return y; }

    /**
     * @brief Provides a textual description of the object.
     * @return A string containing detailed information about the object.
     */
    virtual string getInfo() const = 0;
};

/**
 * @class Point
 * @brief Represents a named location or waypoint on the map.
 *
 * Points serve as navigational anchors for routes, destinations,
 * and transport movement.
 * 
 * @example
 * @code
 * Point p("Airport", 100.0, 200.0);
 * cout << p.getName();
 * @endcode
 */
class Point : public MapObject {
    string name;  ///< Human-readable name of the location.

public:
    /**
    * @brief Creates a named point at a given coordinate.
    * @param n Name of the location.
    * @param xCoord X position.
    * @param yCoord Y position.
    */
    Point(string n, double xCoord, double yCoord);

    /// @return The name of the point.
    string getName() const;

    /// @return Full informational string describing the point.
    string getInfo() const override;
};

/**
 * @class Obstacle
 * @brief Represents an obstacle that may interfere with movement.
 *
 * Obstacles can be mountains, storms, traffic jams,
 * or anything that restricts or alters available navigation.
 * 
 * @example
 * @code
 * Obstacle rock("Big rock", 30, 40);
 * cout << rock.getInfo();
 * @endcode
 */
class Obstacle : public MapObject {
    string description;  ///< Type or nature of the obstacle.

public:
    /**
     * @brief Creates an obstacle with a text description.
     * @param desc Description of the obstacle.
     * @param xCoord X location in the environment.
     * @param yCoord Y location in the environment.
     */
    Obstacle(string desc, double xCoord, double yCoord);

    /// @return Human-readable description of the obstacle.
    string getDescription() const;

    /// @return Detailed string representation of the obstacle.
    string getInfo() const override;
};

/**
 * @class Route
 * @brief Represents a directed connection between two Points.
 *
 * A route contains start and destination points, as well as its physical
 * distance. This class is used for path calculations and environment display.
 * 
 * @example
 * @code
 * Point a("A", 0, 0);
 * Point b("B", 10, 5);
 * Route r(a, b, 12.5);
 * r.showRoute();
 * @endcode
 */
class Route {
    Point start;  ///< Starting point of the route.
    Point destination;  ///< Ending point of the route.
    double distance;  ///< Physical or logical distance between the two points.

public:
    /**
     * @brief Creates a route between two points.
     * @param s Starting point.
     * @param d Destination point.
     * @param dist Physical distance between the points.
     */
    Route(Point s, Point d, double dist);

    /// @brief Displays route information to the output stream.
    void showRoute() const;

    /// @return Length of the route.
    double getDistance() const;

    /// @return Starting point of the route.
    Point getStart() const;

    /// @return Destination point of the route.
    Point getDestination() const;
};

/**
 * @class Environment
 * @brief Holds all navigational elements—routes and obstacles.
 *
 * Environment acts as the world model. It supports:
 * - adding and clearing routes/obstacles,
 * - visualizing all map elements,
 * - computing optimal paths via external Graph and Transport classes,
 * - executing movement of Transport objects through a computed path.
 * 
 * @example
 * @code
 * Environment env;
 * env.addRoute(Route(Point("A",0,0), Point("B",10,10), 14));
 * env.showEnvironment();
 * @endcode
 */
class Environment {
    vector<Route> routes;  ///< All navigable routes in the world.
    vector<Obstacle> obstacles;  ///< All physical/environmental obstacles.

public:
    /** @brief Adds a route to the environment.
    * @param route The route to be added to the environment.
    */
    void addRoute(const Route& route);

    /** @brief Adds an obstacle to the environment.
    * @param obs The obstacle to be added to the environment.
    */
    void addObstacle(const Obstacle& obs);

    /// @brief Displays all routes and obstacles.
    void showEnvironment() const;

    /// @return Read-only list of all routes.
    const vector<Route>& getRoutes() const;

    /// @return Read-only list of all obstacles.
    const vector<Obstacle>& getObstacles() const;

    /// @brief Removes all stored routes.
    void clearRoutes();

    /// @brief Removes all stored obstacles.
    void clearObstacles();

    /**
  * @brief Computes the optimal route between two nodes in the graph.
  *
  * This method leverages the provided Graph and Transport implementation.
  * The result is a sequence of graph node indices representing the final path.
  *
  * @param graph The navigation graph containing weighted edges.
  * @param start Index of the starting graph node.
  * @param end Index of the target graph node.
  * @param transport Reference to the transport used for cost calculations.
  * @return A list of node indices forming the optimal route.
  * 
  * @throws std::runtime_error If start or end nodes do not exist.
  * @throws std::logic_error If transport cannot compute movement cost.
  */
    vector<int> findOptimalRoute(Graph<int>& graph, int start, int end, Transport& transport);

    /**
     * @brief Moves the transport along a precomputed path.
     *
     * Each index in the route vector corresponds to a graph node.
     * Implementations may simulate movement, animation, or logging.
     *
     * @param transport The transport object that moves.
     * @param route Sequence of node indices forming the travel path.
     */
    void moveTransport(Transport& transport, const vector<int>& route);
};
