#pragma once

#include "DrawableGameObject.h"
#include "WaypointManager.h"
#include "Waypoint.h"
#include "Vector2D.h"
#include "Collidable.h"
#include "ForceMotion.h"
#include <string>

#define VEHICLE_MASS 0.00005f
#define SEEK_RADIUS 10

enum MessageType
{
	NONE_MESSAGE = 0,
	SEEK_MESSAGE,
	WANDER_MESSAGE
};

typedef struct MessagePosition
{
	Vector2D position;
	MessageType type;
};




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
	void setWaypointManager(WaypointManager* wpm);
	void hasCollided() {}

	ForceMotion* getForceMotion() { return &m_forceMotion; }

	void applyForceToPosition(const Vector2D& positionTo, MessageType type = MessageType::NONE_MESSAGE);

	void brake(Vector2D direction);

#pragma region SteeringBehaviours

	void Wander();

#pragma endregion


protected: // protected methods
	Vector2D* getPositionAddress() { return &m_currentPosition; }

	
	void updateMessages(const float deltaTime);
	void messageReceived(MessagePosition message);
	void addMessage(MessagePosition message);


protected: // protected properties
	Vector2D m_currentPosition;
	Vector2D m_lastPosition;

	Vector2D m_force;

	WaypointManager* m_waypointManager;
	ForceMotion m_forceMotion;

	list<MessagePosition> m_vecMessages;

};

