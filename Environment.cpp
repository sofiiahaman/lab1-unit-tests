#include "Environment.h"
#include "Graph.h"
#include "Transport.h"
#include <cmath>
#include <vector>
#include <iostream>
using namespace std;


// Point (location)
Point::Point(string n, double xCoord, double yCoord) : MapObject(xCoord, yCoord), name(n) {}
string Point::getName() const { return name; }
string Point::getInfo() const { return "Point: " + name; }

// Obstacle
Obstacle::Obstacle(string desc, double xCoord, double yCoord) : MapObject(xCoord, yCoord), description(desc) {}
string Obstacle::getDescription() const { return description; }
string Obstacle::getInfo() const { return "Obstacle: " + description; }

// Route
Route::Route(Point s, Point d, double dist)
    : start(s), destination(d), distance(dist) {
}

void Route::showRoute() const {
    cout << "Route from " << start.getName() << " to " << destination.getName() << " (" << distance << " km)" << endl;
}

double Route::getDistance() const { return distance; }
Point Route::getStart() const { return start; }
Point Route::getDestination() const { return destination; }



// Environment
void Environment::addRoute(const Route& route) {
    routes.push_back(route);
}

void Environment::addObstacle(const Obstacle& obs) {
    obstacles.push_back(obs);
}

void Environment::showEnvironment() const {
    cout << "Environment overview" << endl;

    cout << "\nRoutes:" << endl;
    for (const auto& r : routes) {
        r.showRoute();
    }

    cout << "\nObstacles:" << endl;
    for (const auto& o : obstacles) {
        cout << "- " << o.getDescription()
            << " at (" << o.getX() << ", " << o.getY() << ")" << endl;
    }
    cout << endl;
}

const vector<Route>& Environment::getRoutes() const {
    return routes;
}

const vector<Obstacle>& Environment::getObstacles() const {
    return obstacles;
}

vector<int> Environment::findOptimalRoute(Graph<int>& graph, int start, int end, Transport& transport) {
    cout << "\nFinding optimal route for " << transport.getName() << "...\n";
    auto [path, distance] = graph.shortest_path(start, end, true);
    cout << "Optimal route: ";
    for (int v : path) cout << v << " ";
    cout << endl;
    return path;
}

void Environment::moveTransport(Transport& transport, const vector<int>& route) {
    cout << "\n" << transport.getName() << " moves along the route: ";
    for (int v : route) cout << v << " ";
    cout << endl;
}