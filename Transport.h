/**
 * @file Transport.h
 * @author Sofiia Haman
 * @date 18.11.2025
 * @version 1.0
 * @brief Base classes for various types of transport (land, water, air).
 *
 * @details Defines a set of base and derived classes representing various types of transport:
 * - Transport: The base class for all transport types, with properties such as speed, position, and fuel.
 * - LandTransport: A derived class for transport that operates on land, including cars and trains.
 * - WaterTransport: A derived class for transport that operates on water, such as yachts.
 * - AirTransport: A derived class for air transport, including helicopters and airplanes.
 * These classes support behaviors such as movement, acceleration, and fuel management.
 */

#pragma once
#include <iostream>
#include <string>
using namespace std;

/**
 * @class Transport
 * @brief Base class for all types of transport.
 *
 * Contains main characteristics and behavior:
 * speed, current position, movement, braking, acceleration.
 *
 * @example
 * @code
 * Transport* t = new Transport("Generic", 50.0);
 * t->move(10);
 * t->info();
 * delete t;
 * @endcode
 */
class Transport {
protected:
    string name;  ///< Transport name
    double speed;  ///< Current speed (km/h)
    double position;  ///< Position on the route (km)
public:
    /**
     * @brief Constructs a transport object.
     * @param n Name of the transport.
     * @param s Speed.
     */
    Transport(string n, double s);

    /**
    * @brief Gets the name of the transport.
    * @return Transport name.
    */
    string getName() const;

    /**
     * @brief Moves transport by a certain distance.
     * @param distance Distance in km.
     * @throws std::runtime_error If transport has no fuel.
     */
    virtual void move(double distance);

    /// @brief Outputs information about the transport.
    virtual void info() const;

    /// @brief Virtual destructor.
    virtual ~Transport();

    /**
     * @brief Accelerates the transport.
     * @param increment Increase in speed (km/h).
     */
    virtual void accelerate(double increment);

    /**
     * @brief Slows down the transport.
     * @param decrement Decrease in speed (km/h).
     */
    virtual void brake(double decrement);

    /**
    * @brief Checks whether the transport has fuel.
    * @return True if fuel is available.
    */
    virtual bool hasFuel() const { return true; }

    /**
     * @brief Updates the current position of the transport.
     * @param distance Distance traveled.
     */
    void updatePosition(double distance);

    /**
    * @brief Gets current position.
    * @return Position in km.
    */
    double getPosition() const { return position; }

    /**
    * @brief Gets the current speed.
    * @return Speed in km/h.
    */
    double getSpeed() const { return speed; }

    /**
    * @brief Sets the current fuel amount.
    * @param amount Fuel amount.
    */
    virtual void setFuel(double) {}

    /**
    * @brief Gets the current fuel level.
    * @return Fuel level.
    */
    virtual double getFuel() const { return 0.0; }
};

/**
* @class LandTransport
* @brief Represents transport that moves on land.
*
* Includes wheels, fuel consumption, and related behavior.
*/
class LandTransport : public Transport {
protected:
    int wheels;  ///< Number of wheels
    double fuelCapacity;  ///< Maximum fuel capacity
    double currentFuel;  ///< Current fuel level (liters)
public:
    /**
     * @brief Creates land transport.
     * @param n Name.
     * @param s Speed.
     * @param w Wheel count.
     * @param fuelCap Fuel tank capacity.
     */
    LandTransport(string n, double s, int w, double fuelCap);

    /**
     * @brief Moves transport with fuel consumption.
     * @param distance Distance to move in km.
     * @throws std::runtime_error If no fuel.
     */
    void move(double distance) override;

    /// @brief Outputs information about the land transport.
    void info() const override;

    /**
    * @brief Accelerates the land transport.
    * @param increment Speed increase.
    */
    void accelerate(double increment) override;

    /**
    * @brief Brakes the land transport.
    * @param decrement Speed decrease.
    */
    void brake(double decrement) override;

    /**
     * @brief Checks if the transport has fuel.
     * @return True if fuel is available.
     */
    bool hasFuel() const override;

    /**
    * @brief Gets the current speed of the land transport.
    * @return Speed in km/h.
    */
    double getSpeed() const { return speed; }

    /**
    * @brief Gets the number of wheels.
    * @return Wheel count.
    */
    int getWheels() const { return wheels; }

    /**
    * @brief Gets current fuel level.
    * @return Amount of fuel in liters.
    */
    double getFuelLevel() const { return currentFuel; }

    /**
    * @brief Sets the current fuel amount.
    * @param amount Amount of fuel to set.
    */
    void setFuel(double amount) { currentFuel = std::max(0.0, std::min(amount, fuelCapacity)); }
};

/**
 * @class WaterTransport
 * @brief Represents transport that moves on water.
 *
 * Extends the base Transport class by adding properties such as propulsion type,
 * fuel capacity, and current fuel level. Water transport consumes fuel while moving.
 */
class WaterTransport : public Transport {
protected:
    string propulsion;  ///< Type of propulsion (e.g., motor, sail)
    double fuelCapacity;  ///< Maximum fuel capacity (liters)
    double currentFuel;  ///< Current fuel level (liters)
public:
    /**
     * @brief Constructs a water transport object.
     * @param n Name of the vessel.
     * @param s Speed in km/h.
     * @param p Propulsion type.
     * @param fuelCap Maximum fuel capacity.
     */
    WaterTransport(string n, double s, string p, double fuelCap);

    /**
     * @brief Moves the water transport by a given distance.
     *
     * Updates position and consumes fuel. If there is no fuel,
     * throws an exception.
     *
     * @param distance Distance to move in km.
     * @throws std::runtime_error If there is no fuel.
     */
    void move(double distance) override;

    /// @brief Displays detailed information about the water transport.
    void info() const override;

    /**
     * @brief Checks whether the vessel has fuel.
     * @return True if fuel is available, false otherwise.
     */
    bool hasFuel() const override;

    /**
     * @brief Gets the propulsion system of the vessel.
     * @return String describing the propulsion type.
     */
	string getPropulsion() const { return propulsion; }

    /**
     * @brief Gets the current fuel level.
     * @return Fuel amount in liters.
     */
    double getFuelLevel() const { return currentFuel; }

    /**
     * @brief Sets the current fuel level.
     *
     * Ensures the assigned value stays within [0, fuelCapacity].
     *
     * @param amount New fuel amount (liters).
     */
    void setFuel(double amount) { currentFuel = std::max(0.0, std::min(amount, fuelCapacity)); }
};

/**
 * @class AirTransport
 * @brief Represents transport that moves through the air.
 *
 * Extends the Transport class by adding altitude handling and fuel management
 * specific to aircraft. Air transport consumes fuel while moving and operates
 * at a defined altitude.
 */
class AirTransport : public Transport {
protected:
    double altitude;  ///< Current altitude in meters
    double fuelCapacity;  ///< Maximum available fuel capacity (liters)
    double currentFuel;  ///< Current fuel level (liters)
public:
    /**
     * @brief Constructs an air transport object.
     *
     * @param n Name of the aircraft.
     * @param s Speed in km/h.
     * @param a Initial altitude in meters.
     * @param fuelCap Maximum fuel capacity.
     */
    AirTransport(string n, double s, double a, double fuelCap);

    /**
     * @brief Moves the aircraft a specified distance.
     *
     * Updates the aircraft's position and consumes fuel proportional to
     * the distance traveled. If no fuel is available, movement is not allowed.
     *
     * @param distance Distance to travel in km.
     * @throws std::runtime_error If fuel is insufficient.
     */
    void move(double distance) override;

    /// @brief Outputs detailed information about the aircraft.
    void info() const override;

    /**
     * @brief Checks whether the aircraft has fuel.
     *
     * @return True if there is fuel remaining, false otherwise.
     */
    bool hasFuel() const override;

    /**
     * @brief Gets the current altitude.
     * @return Altitude in meters.
     */
    double getAltitude() const { return altitude; }

    /**
     * @brief Gets the current fuel level.
     * @return Fuel amount in liters.
     */

    double getFuelLevel() const { return currentFuel; }

    /**
     * @brief Sets the current fuel amount.
     *
     * The value is clamped to the valid range [0, fuelCapacity].
     *
     * @param amount Fuel amount to set.
     */
    void setFuel(double amount) { currentFuel = std::max(0.0, std::min(amount, fuelCapacity)); }
};


/**
 * @class Car
 * @brief Represents a land vehicle powered by fuel.
 *
 * Inherits from LandTransport and adds specific characteristics such as
 * fuel type and consumption rate. A car consumes fuel proportionally to
 * the distance traveled and cannot move without fuel.
 */
class Car : public LandTransport {
    string fuelType;  ///< Type of fuel used
    double fuelConsumptionRate;  ///< Fuel consumption rate (liters per km)
public:
    /**
     * @brief Constructs a car object.
     *
     * @param n Name of the car.
     * @param s Speed in km/h.
     * @param w Number of wheels.
     * @param fuel Fuel type.
     * @param fuelCap Maximum fuel capacity in liters.
     * @param consumptionRate Fuel consumption rate (liters per km).
     */
    Car(string n, double s, int w, string fuel, double fuelCap, double consumptionRate);

    /**
     * @brief Moves the car a given distance.
     *
     * Consumes fuel based on the fuel consumption rate.
     *
     * @param distance Distance to travel in km.
     * @throws std::runtime_error If there is not enough fuel.
     */
    void move(double distance) override;

    /// @brief Displays detailed information about the car.
    void info() const override;

    /**
     * @brief Gets the current fuel level.
     * @return Fuel level in liters.
     */
    double getFuelLevel() const;

    /**
     * @brief Gets the current speed of the car.
     * @return Speed in km/h.
     */
    double getSpeed() const;

    /**
     * @brief Gets the type of fuel the car uses.
     * @return Fuel type as a string.
     */
	string getFuelType() const { return fuelType; }

    /**
     * @brief Gets the car's fuel consumption rate.
     * @return Fuel consumption in liters per km.
     */
	double getFuelConsumptionRate() const { return fuelConsumptionRate; }

    /**
     * @brief Sets the current fuel amount.
     *
     * Ensures the value does not exceed the fuel capacity or fall below zero.
     *
     * @param amount Fuel amount to set in liters.
     */
    void setFuel(double amount) { currentFuel = std::max(0.0, std::min(amount, fuelCapacity)); }
};

/**
 * @class Train
 * @brief Represents a land-based train with multiple carriages.
 *
 * Inherits from LandTransport and adds characteristics specific to trains,
 * such as the number of carriages and a dedicated fuel consumption rate.
 * A train consumes fuel depending on the distance traveled and cannot operate
 * without sufficient fuel.
 */
class Train : public LandTransport {
    int carriages;  ///< Number of carriages attached to the train
    double fuelConsumptionRate;  ///< Fuel consumption rate (liters per km)
public:
    /**
     * @brief Constructs a train object.
     *
     * @param n Name of the train.
     * @param s Speed in km/h.
     * @param w Number of wheels.
     * @param c Number of carriages.
     * @param fuelCap Maximum fuel capacity in liters.
     * @param consumptionRate Fuel consumption rate (liters per km).
     */
    Train(string n, double s, int w, int c, double fuelCap, double consumptionRate);

    /**
     * @brief Moves the train by a specified distance.
     *
     * Consumes fuel proportionally to the fuel consumption rate.
     * Throws an exception if the train does not have enough fuel.
     *
     * @param distance Distance to travel in km.
     * @throws std::runtime_error If insufficient fuel is available.
     */
    void move(double distance) override;

    /// @brief Outputs detailed information about the train.
    void info() const override;

    /**
     * @brief Gets the current fuel level.
     * @return Fuel amount in liters.
     */
    double getFuelLevel() const;

    /**
     * @brief Gets the current speed of the train.
     * @return Speed in km/h.
     */
    double getSpeed() const;

    /**
     * @brief Gets the number of carriages.
     * @return Number of train carriages.
     */
	int getCarriages() const { return carriages; }

    /**
     * @brief Gets the train's fuel consumption rate.
     * @return Fuel consumption rate in liters per km.
     */
	double getFuelConsumptionRate() const { return fuelConsumptionRate; }

    /**
     * @brief Sets the current fuel amount.
     *
     * Ensures the new amount does not exceed the fuel capacity nor fall below zero.
     *
     * @param amount New fuel level in liters.
     */
    void setFuel(double amount) { currentFuel = std::max(0.0, std::min(amount, fuelCapacity)); }
};

/**
 * @class Yacht
 * @brief Represents a water-based luxury vessel with cabins.
 *
 * Inherits from WaterTransport and adds yacht-specific attributes such as
 * cabin count and a dedicated fuel consumption rate. A yacht consumes fuel
 * while moving and cannot travel without sufficient fuel.
 */
class Yacht : public WaterTransport {
    int cabins;  ///< Number of cabins onboard
    double fuelConsumptionRate;  ///< Fuel consumption rate (liters per km)
public:
    /**
     * @brief Constructs a yacht object.
     *
     * @param n Name of the yacht.
     * @param s Speed in km/h.
     * @param p Propulsion type (e.g., motor, hybrid).
     * @param c Number of cabins.
     * @param fuelCap Maximum fuel capacity (liters).
     * @param consumptionRate Fuel consumption rate (liters per km).
     */
    Yacht(string n, double s, string p, int c, double fuelCap, double consumptionRate);

    /**
     * @brief Moves the yacht a given distance.
     *
     * Consumes fuel according to the fuel consumption rate.
     * If insufficient fuel is available, movement is not permitted.
     *
     * @param distance Distance to travel in km.
     * @throws std::runtime_error If there is not enough fuel.
     */
    void move(double distance) override;

    /// @brief Outputs detailed information about the yacht.
    void info() const override;

    /**
     * @brief Gets the current fuel level.
     * @return Fuel level in liters.
     */
    double getFuelLevel() const;

    /**
     * @brief Gets the current speed of the yacht.
     * @return Speed in km/h.
     */
    double getSpeed() const;

    /**
     * @brief Gets the propulsion type of the yacht.
     * @return Propulsion system name.
     */
	string getPropulsion() const { return propulsion; }

    /**
     * @brief Gets the number of cabins on the yacht.
     * @return Number of cabins.
     */
	int getCabins() const { return cabins; }

    /**
     * @brief Gets the yacht’s fuel consumption rate.
     * @return Fuel consumption in liters per km.
     */
	double getFuelConsumptionRate() const { return fuelConsumptionRate; }

    /**
     * @brief Sets the current fuel level.
     *
     * Ensures the value stays within the valid range [0, fuelCapacity].
     *
     * @param amount Fuel amount in liters.
     */
    void setFuel(double amount) { currentFuel = std::max(0.0, std::min(amount, fuelCapacity)); }
};

/**
 * @class Helicopter
 * @brief Represents an air transport vehicle capable of vertical takeoff and landing.
 *
 * Inherits from AirTransport and adds helicopter-specific attributes such as
 * passenger capacity and a dedicated fuel consumption rate. A helicopter
 * consumes fuel while traveling and cannot move without sufficient fuel.
 */
class Helicopter : public AirTransport {
    int passengers;  ///< Number of passengers the helicopter can carry
    double fuelConsumptionRate;  ///< Fuel consumption rate (liters per km)
public:
    /**
     * @brief Constructs a helicopter object.
     *
     * @param n Name of the helicopter.
     * @param s Speed in km/h.
     * @param a Operating altitude in meters.
     * @param p Passenger capacity.
     * @param fuelCap Maximum fuel capacity (liters).
     * @param consumptionRate Fuel consumption rate (liters per km).
     */
    Helicopter(string n, double s, double a, int p, double fuelCap, double consumptionRate);

    /**
     * @brief Moves the helicopter a given distance.
     *
     * Consumes fuel based on the fuel consumption rate.
     * Movement is not allowed if the helicopter does not have enough fuel.
     *
     * @param distance Distance in km.
     * @throws std::runtime_error If insufficient fuel is available.
     */
    void move(double distance) override;

    /// @brief Outputs detailed information about the helicopter.
    void info() const override;

    /**
     * @brief Gets the current fuel level.
     * @return Fuel level in liters.
     */
    double getFuelLevel() const;

    /**
     * @brief Gets the current speed of the helicopter.
     * @return Speed in km/h.
     */
    double getSpeed() const;

    /**
     * @brief Gets the passenger capacity of the helicopter.
     * @return Number of passengers.
     */
	int getPassengers() const { return passengers; }

    /**
     * @brief Gets the helicopter's fuel consumption rate.
     * @return Fuel consumption rate in liters per km.
     */
    double getFuelConsumptionRate() const { return fuelConsumptionRate; }

    /**
     * @brief Sets the current fuel amount.
     *
     * The value is clamped between 0 and the fuel capacity.
     *
     * @param amount Fuel amount in liters.
     */
    void setFuel(double amount) { currentFuel = std::max(0.0, std::min(amount, fuelCapacity)); }
};
