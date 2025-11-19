/**
 * @file Transport.cpp
 * @author Sofiia Haman
 * @date 18.11.2025
 * @version 1.0
 * @brief Implementation of transport classes including the base Transport class,
 *        specialized land, water, and air transport types, and detailed movement
 *        and fuel consumption logic for each transport category.
 */


#include "Transport.h"

//==================== Base Transport ====================
// Position starts at 0, representing the initial location.
Transport::Transport(string n, double s) : name(n), speed(s), position(0) {}
string Transport::getName() const { return name; }

void Transport::move(double distance) {
    cout << name << " moves " << distance << " km at speed " << speed << " km/h." << endl;
    updatePosition(distance);
}

void Transport::info() const {
    cout << "Name: " << name << ", speed: " << speed << " km/h, position: " << position << " km" << endl;
}

Transport::~Transport() {}

void Transport::accelerate(double increment) {
    speed += increment;
    cout << name << " accelerates to " << speed << " km/h." << endl;
}
void Transport::brake(double decrement) {
    speed -= decrement;
    if (speed < 0) speed = 0;
    cout << name << " slows down to " << speed << " km/h." << endl;
}
void Transport::updatePosition(double distance) {
    position += distance;
}

//==================== Land Transport ====================
// Starts with a full fuel tank.
LandTransport::LandTransport(string n, double s, int w, double fuelCap)
    : Transport(n, s), wheels(w), fuelCapacity(fuelCap), currentFuel(fuelCap) {
}

bool LandTransport::hasFuel() const {
    return currentFuel > 0;
}

void LandTransport::move(double distance) {
    if (!hasFuel()) {
        cout << name << " cannot move: Out of fuel." << endl;
        return;
    }
    cout << name << " drives on land with " << wheels << " wheels." << endl;
    updatePosition(distance);
}

void LandTransport::info() const {
    Transport::info();
    cout << "Number of wheels: " << wheels << ", Fuel: " << currentFuel << "/" << fuelCapacity << " liters" << endl;
}

void LandTransport::accelerate(double increment) {
    Transport::accelerate(increment);
}

void LandTransport::brake(double decrement) {
    Transport::brake(decrement);
}

//==================== Water Transport ====================
WaterTransport::WaterTransport(string n, double s, string p, double fuelCap)
    : Transport(n, s), propulsion(p), fuelCapacity(fuelCap), currentFuel(fuelCap) {
}

bool WaterTransport::hasFuel() const {
    return currentFuel > 0;
}

void WaterTransport::move(double distance) {
    if (!hasFuel()) {
        cout << name << " cannot move: Out of fuel." << endl;
        return;
    }
    updatePosition(distance);
    cout << name << " sails on water using " << propulsion << ", moved " << distance << " km." << endl;
}

void WaterTransport::info() const {
    Transport::info();
    cout << "Propulsion type: " << propulsion << ", Fuel: " << currentFuel << "/" << fuelCapacity << " liters" << endl;
}

//==================== Air Transport ====================
AirTransport::AirTransport(string n, double s, double a, double fuelCap)
    : Transport(n, s), altitude(a), fuelCapacity(fuelCap), currentFuel(fuelCap) {
}

bool AirTransport::hasFuel() const {
    return currentFuel > 0;
}

void AirTransport::move(double distance) {
    if (!hasFuel()) {
        cout << name << " cannot move: Out of fuel." << endl;
        return;
    }
    updatePosition(distance);
    cout << name << " flies at an altitude of " << altitude << " meters, moved " << distance << " km." << endl;
}

void AirTransport::info() const {
    Transport::info();
    cout << "Maximum flight altitude: " << altitude << " m, Fuel: " << currentFuel << "/" << fuelCapacity << " liters" << endl;
}

//==================== Car ====================
Car::Car(string n, double s, int w, string fuel, double fuelCap, double consumptionRate)
    : LandTransport(n, s, w, fuelCap), fuelType(fuel), fuelConsumptionRate(consumptionRate) {
}

void Car::move(double distance) {
    if (!hasFuel()) {
        cout << name << " cannot move: Out of fuel." << endl;
        return;
    }
    double fuelNeeded = distance * fuelConsumptionRate;

    // If not enough fuel, reduce travel distance accordingly.
    if (fuelNeeded > currentFuel) {
        cout << name << " does not have enough fuel to move " << distance << " km." << endl;
        distance = currentFuel / fuelConsumptionRate;
        cout << name << " will move only " << distance << " km." << endl;
    }
    currentFuel -= distance * fuelConsumptionRate;
    cout << name << " drives on the road using " << fuelType << ", distance moved: " << distance << " km." << endl;
    updatePosition(distance);
}

void Car::info() const {
    LandTransport::info();
    cout << "Fuel type: " << fuelType << ", Consumption rate: " << fuelConsumptionRate << " L/km" << endl;
}

double Car::getFuelLevel() const {
    return currentFuel;
}

double Car::getSpeed() const {
    return speed;
}

//==================== Train ====================
Train::Train(string n, double s, int w, int c, double fuelCap, double consumptionRate)
    : LandTransport(n, s, w, fuelCap), carriages(c), fuelConsumptionRate(consumptionRate) {
}

void Train::move(double distance) {
    if (!hasFuel()) {
        cout << name << " cannot move: Out of fuel." << endl;
        return;
    }
    double fuelNeeded = distance * fuelConsumptionRate;
    if (fuelNeeded > currentFuel) {
        cout << name << " does not have enough fuel to move " << distance << " km." << endl;
        distance = currentFuel / fuelConsumptionRate;
        cout << name << " will move only " << distance << " km." << endl;
    }
    currentFuel -= distance * fuelConsumptionRate;
    cout << name << " runs on rails with " << carriages << " carriages, moved " << distance << " km." << endl;
    updatePosition(distance);
}

void Train::info() const {
    LandTransport::info();
    cout << "Number of carriages: " << carriages << ", Fuel consumption rate: " << fuelConsumptionRate << " L/km" << endl;
}

double Train::getFuelLevel() const {
    return currentFuel;
}

double Train::getSpeed() const {
    return speed;
}

//==================== Yacht ====================
Yacht::Yacht(string n, double s, string p, int c, double fuelCap, double consumptionRate)
    : WaterTransport(n, s, p, fuelCap), cabins(c), fuelConsumptionRate(consumptionRate) {
}

void Yacht::move(double distance) {
    if (!hasFuel()) {
        cout << name << " cannot move: Out of fuel." << endl;
        return;
    }
    double fuelNeeded = distance * fuelConsumptionRate;
    if (fuelNeeded > currentFuel) {
        cout << name << " does not have enough fuel to move " << distance << " km." << endl;
        distance = currentFuel / fuelConsumptionRate;
        cout << name << " will move only " << distance << " km." << endl;
    }
    currentFuel -= distance * fuelConsumptionRate;
    cout << name << " sails gracefully with " << cabins << " cabins, moved " << distance << " km." << endl;
    updatePosition(distance);
}

void Yacht::info() const {
    WaterTransport::info();
    cout << "Number of cabins: " << cabins << endl;
}

double Yacht::getFuelLevel() const {
    return currentFuel;
}

double Yacht::getSpeed() const {
    return speed;
}

//==================== Helicopter ====================
Helicopter::Helicopter(string n, double s, double a, int p, double fuelCap, double consumptionRate)
    : AirTransport(n, s, a, fuelCap), passengers(p), fuelConsumptionRate(consumptionRate) {
}

void Helicopter::move(double distance) {
    if (!hasFuel()) {
        cout << name << " cannot move: Out of fuel." << endl;
        return;
    }
    double fuelNeeded = distance * fuelConsumptionRate;
    if (fuelNeeded > currentFuel) {
        cout << name << " does not have enough fuel to move " << distance << " km." << endl;
        distance = currentFuel / fuelConsumptionRate;
        cout << name << " will move only " << distance << " km." << endl;
    }
    currentFuel -= distance * fuelConsumptionRate;
    updatePosition(distance);
    cout << name << " flies at " << altitude << " meters altitude with " << passengers << " passengers, moved " << distance << " km." << endl;
}

void Helicopter::info() const {
    AirTransport::info();
    cout << "Number of passengers: " << passengers << ", Fuel consumption rate: " << fuelConsumptionRate << " L/km" << endl;
}

double Helicopter::getFuelLevel() const {
    return currentFuel;
}

double Helicopter::getSpeed() const {
    return speed;
}