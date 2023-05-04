#pragma once

#include "DrawableGameObject.h"
#include "WaypointManager.h"
#include "Waypoint.h"
#include "Vector2D.h"
#include "Collidable.h"
#include "ForceMotion.h"
#include <string>
#include "Whisker.h"
#include "StateManager.h"


#define VEHICLE_MASS 0.00005f

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
	~Vehicle();

public:
	virtual HRESULT initMesh(ID3D11Device* pd3dDevice, carColour colour);
	virtual void update(const float deltaTime);

	void setPosition(Vector2D position); // the current position
	Vector2D* getPositionAddress() { return &m_currentPosition; }
	Vector2D getVelocity() { return m_forceMotion.getVelocity(); }
	/// <returns>The vehicles normalized direction, calculated from the change in position.</returns>
	Vector2D getDirection() { return m_direction; }

	/// <param name="interval">How far ahead, in seconds, to predict the objects position. Passing in delta will get the objects expected position in 1 frame's time, for example</param>
	/// <returns>The expected position that the object will be in "interval" seconds time.</returns>
	Vector2D getPredictedPosition(const float interval);
	/// <returns>A random unit vector.</returns>
	Vector2D getRandomDirection();


	void setWaypointManager(WaypointManager* wpm);
	WaypointManager* getWaypointManager() const { return m_waypointManager; };
	void setTaskManager(TaskManager* taskManager) { m_pTaskManager = taskManager; };
	void setStateManager(StateManager* stateManager) { m_pStateManager = stateManager; };
	void setState(State* state) { if (m_pStateManager != nullptr) m_pStateManager->SetState(state); };
	void hasCollided() {}

	ForceMotion* getForceMotion() { return &m_forceMotion; }

	std::vector<Whisker*> getWhiskers() { return m_whiskers; };


#pragma region Tasks

#pragma region Actions

	void applyForceToPosition(const Vector2D& positionTo);

	void applyForceFromPosition(const Vector2D& positionTo);

	/// <param name="radius">The radius around the predicted position where the random position can be generated</param>
	/// <param name="interval">How many seconds ahead of time to predict the position.</param>
	/// <returns>a random position within a radius of the predicted position</returns>
	Vector2D getWanderPosition(float interval = 0.25f, float radius = 25.0f);

#pragma endregion

#pragma region Checks

	/// <summary>Checks to see if we're within braking distance of a destination. If we are, start slowing as we reach the destination, and come to a stop.</summary>
	/// <param name="destination">The Vector2D of the position we wish to come to a stop at.</param>
	/// <param name="destinationRadiusSquared">The radius, squared, of the zone we consider part of the destination. Somewhat like a margin of error.</param>
	/// <param name="brakingRadiusSquared">The radius, squared, of the zone in which we want to start braking.</param>
	/// <returns>Whether or not we have reached, and stopped at, the destination</returns>
	bool isArrived(Vector2D destination, float destinationRadiusSquared = 2.0f, float brakingRadiusSquared = 4096.0f);
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
	/// <summary>Projects a whisker ahead in the direction of the vehicle, proportional to the current velocity</summary>
	/// <param name="outWhisker">The whisker to modify</param>
	void projectWhisker(Whisker* outWhisker);;
	/// <summary>Projects a whisker at an angle, theta, from the direction of the vehicle, proportional to the current velocity</summary>
	/// <param name="theta">The angle from the vehicle's forward to project the whisker</param>
	/// <param name="outWhisker">The whisker to modify</param>
	void projectWhisker(Whisker* outWhisker, float theta);
	/// <summary>Projects a whisker x distance at an angle, theta, from direction of the vehicle</summary>
	/// <param name="theta">The angle from the vehicle's forward to project the whisker</param>
	/// <param name="distance">The distance to project the whisker forward. Bear in mind the car is 30*20 units</param>
	/// <param name="outWhisker">The whisker to modify</param>
	void projectWhisker(Whisker* outWhisker, float theta, float distance);
	
#pragma endregion

#pragma region SteeringBehaviours

	/// <summary>Get's a random position nearby the predicted position and adds a task that will apply force towards that position each frame, and on completion will call Wander again.</summary>
	void Wander();

	/// <summary>Get's a random waypoint in the area and adds a task that will apply force towards that position each frame, and on completion will call Wander again.</summary>
	void Ramble();

	/// <summary>The Seek steering behavior returns a force that directs an agent toward a target game object's position.
	///<para>We calculate the vector to the target position, Calculate the unit equivalent, And scale it to the maximum speed of the agent.</para></summary>
	/// <param name="soughtObject">The game object whose position will be continually sought.</param>
	void Seek(DrawableGameObject* soughtObject);

	/// <summary>The Seek steering behavior returns a force that directs an agent toward a target position.
	///<para>We calculate the vector to the target position, Calculate the unit equivalent, And scale it to the maximum speed of the agent.</para></summary>
	/// <param name="position">The position to be sought.</param>
	void Seek(Vector2D position);

	/// <summary>The Seek steering behavior returns a force that directs an agent toward a target position, and stops when it reaches that position.
	///<para>We calculate the vector to the target position, Calculate the unit equivalent, And scale it to the maximum speed of the agent.</para></summary>
	/// <param name="position">The position to be sought.</param>
	void Arrive(Vector2D position);

	/// <summary>The Seek steering behavior returns a force that directs an agent toward a target game object's position, and stops when it reaches that position.
	///<para>We calculate the vector to the target position, Calculate the unit equivalent, And scale it to the maximum speed of the agent.</para></summary>
	/// <param name="soughtObject">The game object whose position will be continually sought.</param>
	void Arrive(DrawableGameObject* soughtObject);

	/// <summary>The Flee steering behavior returns a force that directs an agent away from a target game object's position.
	///<para>We calculate the vector from the target position, Calculate the unit equivalent, And scale it to the maximum speed of the agent.</para></summary>
	/// <param name="soughtObject">The game object whose position will be continually fled, while in range.</param>
	void Flee(DrawableGameObject* soughtObject);

	/// <summary>The Seek steering behavior returns a force that directs an agent away from a target position.
	///<para>We calculate the vector from the target position, Calculate the unit equivalent, And scale it to the maximum speed of the agent.</para></summary>
	/// <param name="position">The position to be fled, while in range.</param>
	void Flee(Vector2D position);

	/// <summary>The Evade steering behavior returns a force that directs an agent away from a target game object's predicted position.
	///<para>We calculate the vector from the target position's predicted position, Calculate the unit equivalent, And scale it to the maximum speed of the agent.</para></summary>
	/// <param name="soughtObject">The game object whose position will be continually evaded, while in range.</param>
	void Evade(Vehicle* soughtObject);

	/// <summary>The Intercept steering behavior returns a force that directs an agent away towards a target game object's predicted position.
	///<para>We calculate the vector to the target position's predicted position, Calculate the unit equivalent, And scale it to the maximum speed of the agent.</para></summary>
	/// <param name="soughtObject">The game object whose position will be continually intercepted.</param>
	void Intercept(Vehicle* soughtObject);

#pragma endregion

#pragma endregion


protected: // protected methods

protected: // protected properties
	Vector2D m_currentPosition;
	Vector2D m_lastPosition;
	/// <summary>The direction the vehicle is currently facing, m_currentPosition - m_last position, normalized</summary>
	Vector2D m_direction;

	WaypointManager* m_waypointManager;
	TaskManager* m_pTaskManager;
	StateManager* m_pStateManager;
	ForceMotion m_forceMotion;

	std::vector<Whisker*> m_whiskers;
};

