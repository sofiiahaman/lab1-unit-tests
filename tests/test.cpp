#include "pch.h"
#include "Graph.h"
#include "Transport.h"
#include "Environment.h"
#include <gtest/gtest.h>

class GraphTestFixture : public ::testing::Test {
protected:
    Graph<int> g;

    void SetUp() override {
        g = Graph<int>(false);
    }

    void TearDown() override {
        g = Graph<int>(false);
    }
};

TEST_F(GraphTestFixture, AddAndRemoveVerticesAndEdges) {
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

TEST_F(GraphTestFixture, MSTPrimBasic) {
    Graph<int> g(false);
    g.add_edge(1, 2, 2);
    g.add_edge(1, 3, 3);
    g.add_edge(2, 3, 1);
    g.add_edge(2, 4, 4);
    g.add_edge(3, 4, 5);

    auto [edges, weight] = g.mst_prim(false);

    EXPECT_EQ(weight, 7);
    EXPECT_EQ(edges.size(), 3);
}

TEST_F(GraphTestFixture, MSTKruskal) {
    g.add_edge(10, 20, 5);
    g.add_edge(10, 30, 6);
    g.add_edge(20, 30, 2);
    g.add_edge(30, 40, 3);
    g.add_edge(20, 40, 7);

    auto [edges, weight] = g.mst_kruskal(false);

    EXPECT_EQ(edges.size(), 3);
    EXPECT_EQ(weight, 10);
}

TEST_F(GraphTestFixture, MSTBoruvka) {
    g.add_edge(1, 2, 4);
    g.add_edge(1, 3, 3);
    g.add_edge(2, 3, 2);
    g.add_edge(3, 4, 1);
    g.add_edge(4, 5, 5);

    auto [edges, weight] = g.mst_boruvka(false);

    EXPECT_EQ(edges.size(), 4);
    EXPECT_EQ(weight, 11);
}

TEST_F(GraphTestFixture, HandlesEmptyGraphGracefully) {
    auto [edges, weight] = g.mst_prim(false);
    EXPECT_TRUE(edges.empty());
    EXPECT_EQ(weight, 0);
}

TEST_F(GraphTestFixture, DijkstraShortestPath) {
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

TEST_F(GraphTestFixture, DijkstraNoPath) {
    Graph<int> g(true);
    g.add_edge(1, 2, 5);
    g.add_vertex(3);

    auto [path, dist] = g.shortest_path(1, 3, false);
    EXPECT_EQ(dist, -1);
    EXPECT_TRUE(path.empty());
}

TEST_F(GraphTestFixture, EmptyGraphMST) {
    auto [edges1, weight1] = g.mst_prim(false);
    EXPECT_TRUE(edges1.empty());
    EXPECT_EQ(weight1, 0);

    auto [edges2, weight2] = g.mst_kruskal(false);
    EXPECT_TRUE(edges2.empty());
    EXPECT_EQ(weight2, 0);
}

TEST_F(GraphTestFixture, DirectedGraphMSTNotAllowed) {
    Graph<int> g(true);
    g.add_edge(1, 2, 10);

    auto [edgesPrim, weightPrim] = g.mst_prim(false);
    EXPECT_TRUE(edgesPrim.empty());
    EXPECT_EQ(weightPrim, 0);
}

TEST_F(GraphTestFixture, LoopEdgeIgnored) {
    g.add_edge(1, 1, 10);
    g.add_edge(1, 2, 1);
    g.add_edge(2, 3, 2);

    auto [edges, weight] = g.mst_prim(false);
    EXPECT_EQ(weight, 3);
    EXPECT_EQ(edges.size(), 2);

    std::set<std::pair<int, int>> expectedEdges = { {1,2}, {2,3} };
    std::set<std::pair<int, int>> actualEdges(edges.begin(), edges.end());
    EXPECT_EQ(actualEdges, expectedEdges);
}

TEST_F(GraphTestFixture, NoPathExists) {
    g.add_vertex(1);
    g.add_vertex(2);
    auto [path, dist] = g.shortest_path(1, 2, false);
    EXPECT_TRUE(path.empty());
    EXPECT_EQ(dist, -1);
}

class TransportTest : public ::testing::TestWithParam<std::string> {
protected:
    std::unique_ptr<Transport> transport;

    void SetUp() override {
        std::string type = GetParam();
        if (type == "Land")
            transport = std::make_unique<LandTransport>("Car", 100, 4, 50);
        else if (type == "Water")
            transport = std::make_unique<WaterTransport>("Boat", 30, "motor", 50);
        else if (type == "Air")
            transport = std::make_unique<AirTransport>("Jet", 500, 10000, 50);
    }
};

TEST_P(TransportTest, MoveChangesPositionWhenHasFuel) {
    double start = transport->getPosition();
    transport->move(10);
    EXPECT_GT(transport->getPosition(), start);
}

TEST_P(TransportTest, MoveDoesNotChangePositionWhenNoFuel) {
    transport->setFuel(0);
    double start = transport->getPosition();
    transport->move(10);
    EXPECT_NEAR(transport->getPosition(), start, 1e-9);
}

TEST_P(TransportTest, AccelerateAndBrakeAdjustSpeed) {
    transport->accelerate(20);
    EXPECT_GT(transport->getSpeed(), 0);
    transport->brake(transport->getSpeed() + 10);
    EXPECT_NEAR(transport->getSpeed(), 0.0, 1e-9);
}

TEST(TransportTest, UpdatePositionWorksCorrectly) {
    Transport t("Test", 100);
    t.updatePosition(30);
    EXPECT_NEAR(t.getPosition(), 30, 1e-6);
    t.updatePosition(5);
    EXPECT_NEAR(t.getPosition(), 35, 1e-6);
}

TEST(TransportTest, MoveAndSpeedBehavior) {
    Transport t("Generic", 80);
    t.move(10);
    EXPECT_GT(t.getPosition(), 0);
    t.accelerate(20);
    EXPECT_GT(t.getSpeed(), 80);
    t.brake(100);
    EXPECT_NEAR(t.getSpeed(), 0.0, 1e-9);
}

TEST(TransportTest, NegativeSpeedChangeDoesNotCrash) {
    Transport t("Car", 50);
    EXPECT_NO_THROW(t.accelerate(-10));
    EXPECT_NO_THROW(t.brake(-20));
}

TEST_P(TransportTest, NoMovementIfSpeedIsZero) {
    transport->brake(transport->getSpeed() + 10);
    double startPos = transport->getPosition();
    transport->move(10);
    EXPECT_NEAR(transport->getPosition(), startPos, 1e-9);
}

TEST_P(TransportTest, AccelerateDoesNotAllowNegativeSpeed) {
    transport->brake(transport->getSpeed() + 10);
    transport->accelerate(-20);
    EXPECT_GE(transport->getSpeed(), 0);
}

TEST_P(TransportTest, BrakeDoesNotAllowNegativeSpeed) {
    transport->accelerate(30);
    transport->brake(transport->getSpeed() + 1000);
    EXPECT_NEAR(transport->getSpeed(), 0.0, 1e-9);
}

// Parameterized tests for derived Transport classes
class DerivedTransportTest : public ::testing::TestWithParam<std::string> {
protected:
    std::unique_ptr<Transport> transport;

    void SetUp() override {
        std::string type = GetParam();
        if (type == "Land")
            transport = std::make_unique<LandTransport>("Car", 100, 4, 100);
        else if (type == "Water")
            transport = std::make_unique<WaterTransport>("Boat", 30, "motor", 200);
        else if (type == "Air")
            transport = std::make_unique<AirTransport>("Jet", 500, 10000, 400);
        else if (type == "Car")
            transport = std::make_unique<Car>("Audi", 120, 4, "Gasoline", 5, 1.0);
        else if (type == "Train")
            transport = std::make_unique<Train>("Train", 200, 16, 8, 20, 2.0);
        else if (type == "Yacht")
            transport = std::make_unique<Yacht>("Luxury", 50, "diesel", 4, 10, 1.0);
        else if (type == "Helicopter")
            transport = std::make_unique<Helicopter>("Apache", 250, 3000, 4, 10, 1.0);
    }
};

TEST_P(DerivedTransportTest, MovesWhenHasFuel) {
    double start = transport->getPosition();
    transport->move(10);
    EXPECT_GT(transport->getPosition(), start)
        << "Transport should move forward when it has fuel.";
}

TEST_P(DerivedTransportTest, DoesNotMoveWhenNoFuel) {
    transport->setFuel(0);
    double start = transport->getPosition();
    transport->move(10);
    EXPECT_NEAR(transport->getPosition(), start, 1e-6)
        << "Transport should not move when fuel is empty.";
}

TEST_P(DerivedTransportTest, AccelerateAndBrakeBehavior) {
    transport->accelerate(20);
    EXPECT_GT(transport->getSpeed(), 0)
        << "Transport should increase speed when accelerating.";

    transport->brake(transport->getSpeed() + 50);
    EXPECT_NEAR(transport->getSpeed(), 0.0, 1e-9)
        << "Transport speed should return to zero after braking hard.";
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

class EnvironmentTestFixture : public ::testing::Test {
protected:
    Environment env;

    void SetUp() override {
        env = Environment();
    }

    void TearDown() override {
        env.clearRoutes();
        env.clearObstacles();
    }
};

TEST_F(EnvironmentTestFixture, AddAndGetRoutesWorkCorrectly) {
    Environment env;
    Point a("A", 0, 0);
    Point b("B", 10, 10);
    Route r(a, b, 12.3);
    env.addRoute(r);

    const auto& routes = env.getRoutes();
    ASSERT_EQ(routes.size(), 1);
    EXPECT_EQ(routes[0].getStart().getName(), "A");
}

TEST_F(EnvironmentTestFixture, AddAndGetObstaclesWorkCorrectly) {
    Obstacle o("Storm", 5.0, 7.0);
    env.addObstacle(o);

    const auto& obstacles = env.getObstacles();
    ASSERT_EQ(obstacles.size(), 1);
    EXPECT_EQ(obstacles[0].getDescription(), "Storm");
}

TEST_F(EnvironmentTestFixture, ShowEnvironmentOutputsRoutesAndObstacles) {
    env.addRoute(Route(Point("A", 0, 0), Point("B", 1, 1), 10.0));
    env.addObstacle(Obstacle("Hill", 2.0, 3.0));

    std::ostringstream oss;
    std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());
    env.showEnvironment();
    std::cout.rdbuf(oldCout);

    std::string output = oss.str();
    EXPECT_NE(output.find("Route"), std::string::npos);
    EXPECT_NE(output.find("Hill"), std::string::npos);
}

TEST_F(EnvironmentTestFixture, FindOptimalRouteReturnsCorrectPath) {
    Graph<int> graph(false);

    graph.add_edge(1, 2, 5);
    graph.add_edge(2, 3, 3);
    graph.add_edge(1, 3, 10);
    graph.add_edge(3, 4, 2);

    class TestTransport : public Transport {
    public:
        TestTransport(std::string name) : Transport(name, 100.0) {}
        void move(double) override {}
    } car("TestCar");

    std::vector<int> path = env.findOptimalRoute(graph, 1, 4, car);
    std::vector<int> expected = { 1, 2, 3, 4 };
    EXPECT_EQ(path, expected);
}

TEST_F(EnvironmentTestFixture, FindOptimalRouteEmptyGraphReturnsEmpty) {
    Graph<int> graph(false);
    class DummyTransport : public Transport {
    public:
        DummyTransport(std::string n) : Transport(n, 100) {}
        void move(double) override {}
    } car("TestCar");

    std::vector<int> path = env.findOptimalRoute(graph, 1, 4, car);
    EXPECT_TRUE(path.empty());
}

TEST_F(EnvironmentTestFixture, FindOptimalRouteInvalidNodesThrows) {
    Graph<int> graph(false);
    graph.add_edge(1, 2, 5);

    class DummyTransport : public Transport {
    public:
        DummyTransport(std::string n) : Transport(n, 100) {}
        void move(double) override {}
    } car("TestCar");

    EXPECT_THROW(env.findOptimalRoute(graph, 99, 100, car), std::out_of_range);
}

TEST_F(EnvironmentTestFixture, FindOptimalRouteNoPathExists) {
    Graph<int> graph(false);
    graph.add_edge(1, 2, 5);
    graph.add_vertex(3);

    class TestTransport : public Transport {
    public:
        TestTransport(std::string name) : Transport(name, 100.0) {}
        void move(double) override {}
    } drone("Drone");

    std::vector<int> path = env.findOptimalRoute(graph, 1, 3, drone);
    EXPECT_TRUE(path.empty());
}

TEST_F(EnvironmentTestFixture, MoveTransportPrintsContainsRouteInfo) {
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