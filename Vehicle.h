#pragma once

#include "DrawableGameObject.h"
#include "WaypointManager.h"
#include "Waypoint.h"
#include "Vector2D.h"
#include "Collidable.h"
#include "ForceMotion.h"
#include <string>
#include "Tasking.h"


#define VEHICLE_MASS 0.00005f
#define SEEK_RADIUS 10

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

	void brake(Vector2D direction);

#pragma region SteeringBehaviours

	void Wander();

	bool isArrived(Vector2D destination);

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

	Vector2D m_force;

	WaypointManager* m_waypointManager;
	TaskManager* m_pTaskManager;
	ForceMotion m_forceMotion;
};

