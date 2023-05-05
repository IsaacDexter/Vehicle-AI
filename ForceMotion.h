#pragma once

#include "Vector2D.h"

//https://en.wikipedia.org/wiki/Gravitational_acceleration
#define GRAVITATIONAL_ACCELERATION 9.8
//https://en.wikipedia.org/wiki/Density
#define AIR_DENSITY 1.2

//Stats taken from a Ford Fiesta
//https://en.wikipedia.org/wiki/Rolling_resistance
#define ROLLING_RESISTANCE_COEFFICIENT 0.012
//https://ecomodder.com/wiki/Vehicle_Coefficient_of_Drag_List
#define DRAG_COEFFICIENT 0.36
//https://ecomodder.com/wiki/Vehicle_Coefficient_of_Drag_List
#define FRONTAL_AREA 19.80


class ForceMotion {

private:
    float       m_mass; // object's mass
    float       m_weight;   //Objects mass * g
    Vector2D    m_velocity; // object's velocity
    Vector2D    m_acceleration; //objects acc in m/s^2
    Vector2D    m_force; // force applied to the object
    Vector2D*   m_pPosition;
    Vector2D    m_direction;

public:
    // constructor
    ForceMotion(float m, Vector2D* position) {
        m_mass = m;
        m_weight = m * GRAVITATIONAL_ACCELERATION;
        m_velocity = Vector2D(0, 0);
        m_force = Vector2D(0, 0);
        m_pPosition = position;
    }

    Vector2D getForceFromVelocity(float dt)
    {
        Vector2D acceleration = m_velocity / dt;
        Vector2D force = acceleration * m_mass;
        return force;
    }

    // method to clear force - do this each frame
    void clearForce() {
        m_force = Vector2D(0,0);
    }

    void clearAcceleration()
    {
        m_acceleration = Vector2D(0, 0);
    }
    
    void clearVelocity()
    {
        m_velocity = Vector2D(0, 0);
    }

    // method to apply force
    void applyForce(Vector2D f) {
        m_force = (f);
    }

    Vector2D calculateDrag()
    {
        float magnitude = AIR_DENSITY * getVelocity().Length() * FRONTAL_AREA * DRAG_COEFFICIENT;
        Vector2D direction = Vec2DNormalize(m_force) * -1;
        return direction * magnitude;
    }

    void applyDamping(double dt)
    {
        m_velocity *= pow(0.9f, dt);
    }

    // method to accumulate force
    void accumulateForce(Vector2D f) {
        m_force += (f);
    }

    void update(double dt) {
        applyDamping(dt);

        updateAcceleration();
        updateVelocity(dt);
        updatePosition(dt);

        clearForce();
        clearAcceleration();
    }

    void updateAcceleration()
    {
        m_acceleration += m_force / m_mass;
    }

    // method to update object's velocity
    void updateVelocity(double dt) {
        m_velocity += m_acceleration * dt;
    }

    // method to update object's position
    void updatePosition(double dt) {
        Vector2D displacement = m_velocity * dt;
        *m_pPosition += displacement;
    }

    // getter for velocity
    Vector2D getVelocity() {
        return m_velocity;
    }
    // getter for force
    Vector2D getForce() {
        return m_force;
    }

};

