#pragma once

#include "DrawableGameObject.h"
#include "WaypointManager.h"
#include "Waypoint.h"
#include "Vector2D.h"
#include "Collidable.h"
#include "ForceMotion.h"
#include <string>
#include <queue>
#include <functional>

#define VEHICLE_MASS 0.00005f
#define SEEK_RADIUS 10

enum MessageType
{
	NONE_MESSAGE = 0,
	SEEK_MESSAGE,
	ARRIVE_MESSAGE,
	WANDER_MESSAGE
};

typedef struct Message
{
	MessageType type;
	Vector2D position;
	Message(MessageType type, Vector2D position)
	{
		this->position = position;
		this->type = type;
	}
	Message()
	{
		this->type = MessageType::NONE_MESSAGE;
		this->position = Vector2D();
	}
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
	Vector2D* getPositionAddress() { return &m_currentPosition; }

	void setWaypointManager(WaypointManager* wpm);
	void hasCollided() {}

	ForceMotion* getForceMotion() { return &m_forceMotion; }

	void applyForceToPosition(const Vector2D& positionTo, MessageType type = MessageType::NONE_MESSAGE);

	void brake(Vector2D direction);

#pragma region SteeringBehaviours

	void Wander();

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

	void updateMessages(const float deltaTime);
	void messageReceived(Message message);
	void addMessage(Message message);


protected: // protected properties
	Vector2D m_currentPosition;
	Vector2D m_lastPosition;

	Vector2D m_force;

	WaypointManager* m_waypointManager;
	ForceMotion m_forceMotion;

	list<Message> m_messages;
	/// <summary>A queue containing paramaterless lambdas that contain the function calls with parameters in them.
	/// <para>Essentially allows the class to queue tasks to be called at another time, in this case in the update function.</para>
	/// <para>Definitely an unconventional way of doing this, but this is the most versatile and reusable way I could think of:</para>
	/// <para>I want these classes to see more life as more than just cars. It also saves having to do a check each frame to see if we are seeking something.</para></summary>
	std::queue<std::function<void()>> m_tasks;
	//I want to thank fredoverflow's answer( https://stackoverflow.com/a/22109991 ) for helping me understand the concept of a queue of function pointers.
	//I want to thank Johannes Schaub - litb's answer ( https://stackoverflow.com/a/2402607 ) for helping me undestand function pointers to member functions.

};

