#pragma once

#include "DrawableGameObject.h"
#include "WaypointManager.h"
#include "Waypoint.h"
#include "Vector2D.h"
#include "Collidable.h"
#include "ForceMotion.h"
#include <string>


#define VEHICLE_MASS 1200.0f

class TaskManager;

enum class carColour
{
	redCar,
	blueCar,
};

class Vehicle : public DrawableGameObject, public Collidable
{
public:
	Vehicle();

public:
	virtual HRESULT initMesh(ID3D11Device* pd3dDevice, carColour colour);
	virtual void update(const float deltaTime);

	void setPosition(Vector2D position); // the current position
	Vector2D* getPositionAddress() { return &m_currentPosition; }

	void setWaypointManager(WaypointManager* wpm);
	void setTaskManager(TaskManager* taskManager) { m_pTaskManager = taskManager; };
	void hasCollided() {}

	ForceMotion* getForceMotion() { return &m_forceMotion; }

	void applyForceToPosition(const Vector2D& positionTo);

#pragma region SteeringBehaviours

	void Wander();
	
	/// <summary>Checks to see if we're within braking distance of a destination. If we are, start slowing as we reach the destination, and come to a stop.</summary>
	/// <param name="destination">The Vector2D of the position we wish to come to a stop at.</param>
	/// <param name="destinationRadiusSquared">The radius, squared, of the zone we consider part of the destination. Somewhat like a margin of error.</param>
	/// <param name="brakingRadiusSquared">The radius, squared, of the zone in which we want to start braking.</param>
	/// <returns>Whether or not we have reached, and stopped at, the destination</returns>
	bool isArrived(Vector2D destination, float destinationRadiusSquared = 100.0f, float brakingRadiusSquared = 1000.0f);
	/// <summary>Checks to see if we have reached a destination, if we have, don't stop moving, and return true.</summary>
	/// <param name="destination">The Vector2D of the position we want to reach.</param>
	/// <param name="destinationRadiusSquared">The radius, squared, of the zone we consider part of the destination. Somewhat like a margin of error.</param>
	/// <returns>Whether or not we have reached, but not stopped at, our destination.</returns>
	bool isPassed(Vector2D destination, float destinationRadiusSquared = 100.0f);
	/// <summary>Checks to see if we're within braking distance of a destination. If we are, start slowing as we reach the destination.</summary>
	/// <param name="distanceSquared">The distance, squared, between the vehice and the destination.</param>
	/// <param name="brakingRadiusSquared">The radius, squared, of the zone in which we want to start braking.</param>
	/// <returns>Whether or not we are within braking distance or a destination, not if we've come to a stop.</returns>
	bool brake(float distanceSquared, float brakingRadiusSquared = 1000.0f);
	/// <summary>Checks to see if we're within braking distance of a destination. If we are, start slowing as we reach the destination.</summary>
	/// <param name="destination">The destination to centre the braking zone around.</param>
	/// <param name="brakingRadiusSquared">The radius, squared, of the zone in which we want to start braking.</param>
	/// <returns>Whether or not we are within braking distance or a destination, not if we've come to a stop.</returns>
	bool brake(Vector2D destination, float brakingRadiusSquared = 1000.0f);

	/// <summary>The Seek steering behavior returns a force that directs an agent toward a target game object's position.
	///<para>We calculate the vector to the target position, Calculate the unit equivalent, And scale it to the maximum speed of the agent.</para></summary>
	/// <param name="soughtObject">The game object whose position will be continually sought.</param>
	void Seek(DrawableGameObject* soughtObject);

	/// <summary>The Seek steering behavior returns a force that directs an agent toward a target position.
	///<para>We calculate the vector to the target position, Calculate the unit equivalent, And scale it to the maximum speed of the agent.</para></summary>
	/// <param name="position">The position to be sought.</param>
	void Seek(Vector2D position);

#pragma endregion


protected: // protected methods

protected: // protected properties
	Vector2D m_currentPosition;
	Vector2D m_lastPosition;

	WaypointManager* m_waypointManager;
	TaskManager* m_pTaskManager;
	ForceMotion m_forceMotion;
};

