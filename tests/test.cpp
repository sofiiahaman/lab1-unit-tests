#include "pch.h"
#include "Graph.h"
#include "Transport.h"
#include "Environment.h"


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

TEST(TransportTest, MoveUpdatesPosition) {
    Transport t("Base", 50);
    t.move(10);
    EXPECT_DOUBLE_EQ(t.getPosition(), 10);
    t.move(5);
    EXPECT_DOUBLE_EQ(t.getPosition(), 15);
}

TEST(TransportTest, AccelerateAndBrakeAdjustSpeed) {
    Transport t("Bus", 60);
    t.accelerate(20);
    EXPECT_DOUBLE_EQ(t.getSpeed(), 80);
    t.brake(30);
    EXPECT_DOUBLE_EQ(t.getSpeed(), 50);
    t.brake(100);
    EXPECT_DOUBLE_EQ(t.getSpeed(), 0);
}

TEST(TransportTest, UpdatePositionWorksCorrectly) {
    Transport t("Test", 100);
    t.updatePosition(30);
    EXPECT_DOUBLE_EQ(t.getPosition(), 30);
    t.updatePosition(5);
    EXPECT_DOUBLE_EQ(t.getPosition(), 35);
}

TEST(LandTransportTest, MoveChangesPositionWhenHasFuel) {
    LandTransport lt("Car", 100, 4, 100);
    double start = lt.getPosition();
    lt.move(20);
    EXPECT_DOUBLE_EQ(lt.getPosition(), start + 20);
}

TEST(TransportTest, MoveAndSpeedBehavior) {
    Transport t("Generic", 80);
    t.move(10);
    EXPECT_GT(t.getPosition(), 0);
    t.accelerate(20);
    EXPECT_GT(t.getSpeed(), 80);
    t.brake(100);
    EXPECT_EQ(t.getSpeed(), 0);
}

TEST(LandTransportTest, MoveDoesNotChangePositionWhenNoFuel) {
    LandTransport lt("Bike", 60, 2, 0);
    double start = lt.getPosition();
    lt.move(10);
    EXPECT_DOUBLE_EQ(lt.getPosition(), start);
    EXPECT_FALSE(lt.hasFuel());
}

TEST(LandTransportTest, AccelerateAndBrakeWorkProperly) {
    LandTransport lt("Bus", 60, 6, 50);
    lt.accelerate(10);
    EXPECT_DOUBLE_EQ(lt.getSpeed(), 70);
    lt.brake(20);
    EXPECT_DOUBLE_EQ(lt.getSpeed(), 50);
}

TEST(WaterTransportTest, MoveChangesPositionWhenHasFuel) {
    WaterTransport w("Boat", 30, "motor", 200);
    double start = w.getPosition();
    w.move(10);
    EXPECT_DOUBLE_EQ(w.getPosition(), start + 10);
}

TEST(WaterTransportTest, MoveDoesNotChangePositionWhenNoFuel) {
    WaterTransport w("Boat", 30, "motor", 0);
    double start = w.getPosition();
    w.move(10);
    EXPECT_DOUBLE_EQ(w.getPosition(), start);
}

TEST(AirTransportTest, MoveChangesPositionWhenHasFuel) {
    AirTransport a("Jet", 500, 10000, 400);
    double start = a.getPosition();
    a.move(20);
    EXPECT_DOUBLE_EQ(a.getPosition(), start + 20);
}

TEST(AirTransportTest, MoveDoesNotChangePositionWhenNoFuel) {
    AirTransport a("Jet", 500, 10000, 0);
    double start = a.getPosition();
    a.move(20);
    EXPECT_DOUBLE_EQ(a.getPosition(), start);
}

TEST(CarTest, MoveConsumesFuelAndStopsWhenEmpty) {
    Car c("Audi", 120, 4, "Gasoline", 5, 1.0);
    double startPos = c.getPosition();
    c.move(10);
    EXPECT_GT(c.getPosition(), startPos);
    double after = c.getPosition();
    c.move(10);
    EXPECT_EQ(c.getPosition(), after);
}

TEST(TrainTest, MoveConsumesFuelAndStopsWhenEmpty) {
    Train tr("Train", 200, 16, 8, 20, 2.0);
    double start = tr.getPosition();
    tr.move(10);
    EXPECT_GT(tr.getPosition(), start);
    double after = tr.getPosition();
    tr.move(100);
    EXPECT_GE(tr.getPosition(), after);
}

TEST(YachtTest, MovesUntilFuelEnds) {
    Yacht y("Luxury", 50, "diesel", 4, 10, 1.0);
    double start = y.getPosition();
    y.move(5);
    EXPECT_GT(y.getPosition(), start);
    double after = y.getPosition();
    y.move(20);
    EXPECT_GE(y.getPosition(), after);
}

TEST(HelicopterTest, MovesUntilFuelEnds) {
    Helicopter h("Apache", 250, 3000, 4, 10, 1.0);
    double start = h.getPosition();
    h.move(5);
    EXPECT_GT(h.getPosition(), start);
    double after = h.getPosition();
    h.move(20);
    EXPECT_GE(h.getPosition(), after);
}

TEST(RouteTest, ShowRouteOutputsCorrectText) {
    Point a("Start", 0, 0);
    Point b("End", 10, 10);
    Route r(a, b, 20.0);

    std::ostringstream oss;
    std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());

    r.showRoute();

    std::cout.rdbuf(oldCout);
    std::string output = oss.str();
    EXPECT_NE(output.find("Route"), std::string::npos);
    EXPECT_NE(output.find("Start"), std::string::npos);
    EXPECT_NE(output.find("End"), std::string::npos);
}

TEST(EnvironmentTest, AddAndGetRoutesWorkCorrectly) {
    Environment env;
    Point a("A", 0, 0);
    Point b("B", 10, 10);
    Route r(a, b, 12.3);
    env.addRoute(r);

    const auto& routes = env.getRoutes();
    ASSERT_EQ(routes.size(), 1);
    EXPECT_EQ(routes[0].getStart().getName(), "A");
    EXPECT_EQ(routes[0].getDestination().getName(), "B");
}

TEST(EnvironmentTest, AddAndGetObstaclesWorkCorrectly) {
    Environment env;
    Obstacle o("Storm", 5.0, 7.0);
    env.addObstacle(o);

    const auto& obstacles = env.getObstacles();
    ASSERT_EQ(obstacles.size(), 1);
    EXPECT_EQ(obstacles[0].getDescription(), "Storm");
    EXPECT_DOUBLE_EQ(obstacles[0].getX(), 5.0);
    EXPECT_DOUBLE_EQ(obstacles[0].getY(), 7.0);
}

TEST(EnvironmentTest, ShowEnvironmentOutputsRoutesAndObstacles) {
    Environment env;
    env.addRoute(Route(Point("A", 0, 0), Point("B", 1, 1), 10.0));
    env.addObstacle(Obstacle("Hill", 2.0, 3.0));

    std::ostringstream oss;
    std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());
    env.showEnvironment();
    std::cout.rdbuf(oldCout);

    std::string output = oss.str();
    EXPECT_NE(output.find("Route"), std::string::npos);
    EXPECT_NE(output.find("A"), std::string::npos);
    EXPECT_NE(output.find("B"), std::string::npos);
    EXPECT_NE(output.find("Hill"), std::string::npos);
}

class TestTransport : public Transport {
public:
    TestTransport(std::string name) : Transport(name, 100.0) {}
    void move(double distance) override {}
};

TEST(EnvironmentTest, FindOptimalRouteReturnsCorrectPath) {
    Graph<int> graph(false);

    graph.add_edge(1, 2, 5);
    graph.add_edge(2, 3, 3);
    graph.add_edge(1, 3, 10);
    graph.add_edge(3, 4, 2);

    TestTransport car("TestCar");
    Environment env;
    std::vector<int> path = env.findOptimalRoute(graph, 1, 4, car);
    std::vector<int> expected = { 1, 2, 3, 4 };
    EXPECT_EQ(path, expected);
    EXPECT_EQ(path.size(), 4);
}

TEST(EnvironmentTest, FindOptimalRouteNoPathExists) {
    Graph<int> graph(false);
    graph.add_edge(1, 2, 5);
    graph.add_vertex(3);

    TestTransport drone("Drone");
    Environment env;

    std::vector<int> path = env.findOptimalRoute(graph, 1, 3, drone);
    EXPECT_TRUE(path.empty());
}

TEST(EnvironmentTest, MoveTransportPrintsContainsRouteInfo) {
    Environment env;
    class DummyTransport : public Transport {
    public:
        DummyTransport(std::string n) : Transport(std::move(n), 100.0) {}
        void move(double) override {}
    } car("TestCar");
    std::vector<int> route = { 1, 2, 3 };

    std::ostringstream oss;
    std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());
    env.moveTransport(car, route);

    std::cout.rdbuf(oldCout);
    std::string output = oss.str();
    EXPECT_NE(output.find("TestCar"), std::string::npos);
    EXPECT_NE(output.find("route"), std::string::npos);
}