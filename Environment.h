#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Transport.h"
#include "Graph.h" 
using namespace std;

class MapObject {
protected:
    double x;
    double y;
public:
    MapObject(double xCoord, double yCoord) : x(xCoord), y(yCoord) {}
    virtual ~MapObject() = default;
    double getX() const { return x; }
    double getY() const { return y; }
    virtual string getInfo() const = 0;
};

// Point (location)
class Point : public MapObject {
    string name;
public:
    Point(string n, double xCoord, double yCoord);
    string getName() const;
    string getInfo() const override;
};

// Obstacle (e.g., mountain, storm, traffic jam)
class Obstacle : public MapObject {
    string description;
public:
    Obstacle(string desc, double xCoord, double yCoord);
    string getDescription() const;
    string getInfo() const override;
};

// Route between two points
class Route {
    Point start;
    Point destination;
    double distance;
public:
    Route(Point s, Point d, double dist);
    void showRoute() const;
    double getDistance() const;
    Point getStart() const;
    Point getDestination() const;
};


// Environment — includes all routes and obstacles
class Environment {
    vector<Route> routes;
    vector<Obstacle> obstacles;
public:
    void addRoute(const Route& route);
    void addObstacle(const Obstacle& obs);
    void showEnvironment() const;

    const vector<Route>& getRoutes() const;
    const vector<Obstacle>& getObstacles() const;

    vector<int> findOptimalRoute(Graph<int>& graph, int start, int end, Transport& transport);
    void moveTransport(Transport& transport, const vector<int>& route);
};
