#include "pch.h"
#include "Graph.h"
#include "Transport.h"


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

TEST(TransportTest, ConstructorSetsCorrectValues) {
    Transport t("Generic", 80);
    EXPECT_EQ(t.getName(), "Generic");
    EXPECT_DOUBLE_EQ(t.getSpeed(), 80);
    EXPECT_DOUBLE_EQ(t.getPosition(), 0);
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

TEST(LandTransportTest, ConstructorSetsCorrectValues) {
    LandTransport lt("Truck", 80, 6, 200);
    EXPECT_EQ(lt.getName(), "Truck");
    EXPECT_DOUBLE_EQ(lt.getSpeed(), 80);
    EXPECT_DOUBLE_EQ(lt.getPosition(), 0);
    EXPECT_EQ(lt.getWheels(), 6);
    EXPECT_DOUBLE_EQ(lt.getFuelLevel(), 200);
    EXPECT_TRUE(lt.hasFuel());
}

TEST(LandTransportTest, MoveChangesPositionWhenHasFuel) {
    LandTransport lt("Car", 100, 4, 100);
    double start = lt.getPosition();
    lt.move(20);
    EXPECT_DOUBLE_EQ(lt.getPosition(), start + 20);
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

TEST(WaterTransportTest, ConstructorSetsCorrectValues) {
    WaterTransport w("Boat", 40, "motor", 150);
    EXPECT_EQ(w.getName(), "Boat");
    EXPECT_DOUBLE_EQ(w.getSpeed(), 40);
    EXPECT_DOUBLE_EQ(w.getPosition(), 0);
    EXPECT_EQ(w.getPropulsion(), "motor");
    EXPECT_DOUBLE_EQ(w.getFuelLevel(), 150);
    EXPECT_TRUE(w.hasFuel());
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

TEST(AirTransportTest, ConstructorSetsCorrectValues) {
    AirTransport a("Plane", 500, 10000, 300);
    EXPECT_EQ(a.getName(), "Plane");
    EXPECT_DOUBLE_EQ(a.getSpeed(), 500);
    EXPECT_DOUBLE_EQ(a.getPosition(), 0);
    EXPECT_DOUBLE_EQ(a.getAltitude(), 10000);
    EXPECT_DOUBLE_EQ(a.getFuelLevel(), 300);
    EXPECT_TRUE(a.hasFuel());
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

TEST(CarTest, ConstructorSetsCorrectValues) {
    Car c("BMW", 120, 4, "Gasoline", 50, 0.5);
    EXPECT_EQ(c.getName(), "BMW");
    EXPECT_DOUBLE_EQ(c.getSpeed(), 120);
    EXPECT_DOUBLE_EQ(c.getPosition(), 0);
    EXPECT_EQ(c.getWheels(), 4);
    EXPECT_EQ(c.getFuelType(), "Gasoline");
    EXPECT_DOUBLE_EQ(c.getFuelLevel(), 50);
    EXPECT_DOUBLE_EQ(c.getFuelConsumptionRate(), 0.5);
    EXPECT_TRUE(c.hasFuel());
}

TEST(CarTest, MoveConsumesFuelCorrectly) {
    Car c("Audi", 120, 4, "Gasoline", 10, 0.5);
    double startFuel = c.getFuelLevel();
    c.move(10);
    EXPECT_LT(c.getFuelLevel(), startFuel);
}

TEST(CarTest, MoveStopsWhenFuelInsufficient) {
    Car c("OldCar", 80, 4, "Gasoline", 2, 1.0);
    double startPos = c.getPosition();
    c.move(10);
    EXPECT_GT(c.getPosition(), startPos);
    EXPECT_NEAR(c.getFuelLevel(), 0, 1e-6);
}

TEST(TrainTest, ConstructorSetsCorrectValues) {
    Train tr("Express", 200, 16, 8, 500, 2.0);
    EXPECT_EQ(tr.getName(), "Express");
    EXPECT_DOUBLE_EQ(tr.getSpeed(), 200);
    EXPECT_DOUBLE_EQ(tr.getPosition(), 0);
    EXPECT_EQ(tr.getCarriages(), 8);
    EXPECT_DOUBLE_EQ(tr.getFuelLevel(), 500);
    EXPECT_DOUBLE_EQ(tr.getFuelConsumptionRate(), 2.0);
    EXPECT_TRUE(tr.hasFuel());
}

TEST(TrainTest, MoveConsumesFuelCorrectly) {
    Train tr("Train", 150, 12, 5, 100, 1.0);
    double oldFuel = tr.getFuelLevel();
    tr.move(20);
    EXPECT_LT(tr.getFuelLevel(), oldFuel);
}

TEST(TrainTest, MoveStopsWhenFuelInsufficient) {
    Train tr("ShortFuel", 150, 12, 5, 5, 2.0);
    double startPos = tr.getPosition();
    tr.move(10);
    EXPECT_GT(tr.getPosition(), startPos);
    EXPECT_NEAR(tr.getFuelLevel(), 0, 1e-6);
}

TEST(YachtTest, ConstructorSetsCorrectValues) {
    Yacht y("Luxury", 50, "diesel", 4, 100, 0.5);
    EXPECT_EQ(y.getName(), "Luxury");
    EXPECT_DOUBLE_EQ(y.getSpeed(), 50);
    EXPECT_DOUBLE_EQ(y.getPosition(), 0);
    EXPECT_EQ(y.getPropulsion(), "diesel");
    EXPECT_EQ(y.getCabins(), 4);
    EXPECT_DOUBLE_EQ(y.getFuelLevel(), 100);
    EXPECT_DOUBLE_EQ(y.getFuelConsumptionRate(), 0.5);
    EXPECT_TRUE(y.hasFuel());
}

TEST(YachtTest, MoveConsumesFuelCorrectly) {
    Yacht y("Y1", 30, "motor", 2, 20, 1.0);
    double oldFuel = y.getFuelLevel();
    y.move(5);
    EXPECT_LT(y.getFuelLevel(), oldFuel);
}

TEST(YachtTest, MoveStopsWhenFuelInsufficient) {
    Yacht y("Small", 20, "motor", 2, 2, 1.0);
    double start = y.getPosition();
    y.move(10);
    EXPECT_GT(y.getPosition(), start);
    EXPECT_NEAR(y.getFuelLevel(), 0, 1e-6);
}

TEST(HelicopterTest, ConstructorSetsCorrectValues) {
    Helicopter h("Apache", 250, 3000, 4, 80, 1.2);
    EXPECT_EQ(h.getName(), "Apache");
    EXPECT_DOUBLE_EQ(h.getSpeed(), 250);
    EXPECT_DOUBLE_EQ(h.getPosition(), 0);
    EXPECT_DOUBLE_EQ(h.getAltitude(), 3000);
    EXPECT_EQ(h.getPassengers(), 4);
    EXPECT_DOUBLE_EQ(h.getFuelLevel(), 80);
    EXPECT_DOUBLE_EQ(h.getFuelConsumptionRate(), 1.2);
    EXPECT_TRUE(h.hasFuel());
}

TEST(HelicopterTest, MoveConsumesFuelCorrectly) {
    Helicopter h("Apache", 250, 3000, 5, 100, 1.0);
    double oldFuel = h.getFuelLevel();
    h.move(10);
    EXPECT_LT(h.getFuelLevel(), oldFuel);
}

TEST(HelicopterTest, MoveStopsWhenFuelInsufficient) {
    Helicopter h("Light", 200, 2000, 2, 3, 1.0);
    double startPos = h.getPosition();
    h.move(10);
    EXPECT_GT(h.getPosition(), startPos);
    EXPECT_NEAR(h.getFuelLevel(), 0, 1e-6);
}