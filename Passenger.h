#pragma once

class Vehicle;

class Passenger
{
public:
    Passenger()
    {
        m_driver = nullptr;
        m_tip = 0.0f;
    }
    /// <summary>Finds a destination from it's drivers waypoint manager, gets the distance between the driver and the destination to determine the tip, and picks it up.</summary>
    /// <param name="driver">The driver to attempt to pick up this passenger</param>
    void Pickup(Vehicle* driver);
    void Dropoff();
    Vector2D GetDestination() const { return m_destination; }
private:
    Vehicle* m_driver;
    const float m_fuelConsumptionIncrease = 0.125f;
    float m_tip;
    Vector2D m_destination;
};

