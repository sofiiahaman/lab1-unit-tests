#pragma once
#include <iostream>
#include <string>
using namespace std;

// Transport
class Transport {
protected:
    string name;
    double speed; // km/h
    double position; // Position along the route in km
public:
    Transport(string n, double s);
    string getName() const;
    virtual void move(double distance);
    virtual void info() const;
    virtual ~Transport();

    virtual void accelerate(double increment);
    virtual void brake(double decrement);

    virtual bool hasFuel() const { return true; }

    void updatePosition(double distance);
    double getPosition() const { return position; }
    double getSpeed() const { return speed; }
};

// Land transport
class LandTransport : public Transport {
protected:
    int wheels;
    double fuelCapacity;
    double currentFuel; // liters
public:
    LandTransport(string n, double s, int w, double fuelCap);
    void move(double distance) override;
    void info() const override;
    void accelerate(double increment) override;
    void brake(double decrement) override;
    bool hasFuel() const override;
    double getSpeed() const { return speed; }
};

// Water transport
class WaterTransport : public Transport {
protected:
    string propulsion;
    double fuelCapacity;
    double currentFuel;
public:
    WaterTransport(string n, double s, string p, double fuelCap);
    void move(double distance) override;
    void info() const override;
    bool hasFuel() const override;
};

// Air transport
class AirTransport : public Transport {
protected:
    double altitude;
    double fuelCapacity;
    double currentFuel;
public:
    AirTransport(string n, double s, double a, double fuelCap);
    void move(double distance) override;
    void info() const override;
    bool hasFuel() const override;
};


// Car
class Car : public LandTransport {
    string fuelType;
    double fuelConsumptionRate; // liters per km
public:
    Car(string n, double s, int w, string fuel, double fuelCap, double consumptionRate);
    void move(double distance) override;
    void info() const override;
    double getFuelLevel() const;
    double getSpeed() const;
};

// Train
class Train : public LandTransport {
    int carriages;
    double fuelConsumptionRate; // for train
public:
    Train(string n, double s, int w, int c, double fuelCap, double consumptionRate);
    void move(double distance) override;
    void info() const override;
    double getFuelLevel() const;
    double getSpeed() const;
};

// Yacht
class Yacht : public WaterTransport {
    int cabins;
    double fuelConsumptionRate;
public:
    Yacht(string n, double s, string p, int c, double fuelCap, double consumptionRate);
    void move(double distance) override;
    void info() const override;
    double getFuelLevel() const;
    double getSpeed() const;
};

// Helicopter
class Helicopter : public AirTransport {
    int passengers;
    double fuelConsumptionRate; // liters per km
public:
    Helicopter(string n, double s, double a, int p, double fuelCap, double consumptionRate);
    void move(double distance) override;
    void info() const override;
    double getFuelLevel() const;
    double getSpeed() const;
};
