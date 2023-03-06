#include "Vehicle.h"
#include "State.h"

#define VEHICLE_MASS 0.00005f
#define BRAKE_DISTANCE 100.0f	//The distance required to slow to a stop

Vehicle::Vehicle() : m_forceMotion(VEHICLE_MASS, getPositionAddress())
{
	m_currentPosition = Vector2D(0,0);
	m_lastPosition = Vector2D(0, 0);
	m_waypointManager = nullptr;
}

HRESULT	Vehicle::initMesh(ID3D11Device* pd3dDevice, carColour colour)
{
	m_scale = XMFLOAT3(30, 20, 1);

	if (colour == carColour::redCar)
	{
		setTextureName(L"Resources\\car_red.dds");
	}
	else if (colour == carColour::blueCar)
	{
		setTextureName(L"Resources\\car_blue.dds");
	}

	HRESULT hr = DrawableGameObject::initMesh(pd3dDevice);

	setPosition(Vector2D(0, 0));

	m_lastPosition = Vector2D(0, 0);

	return hr;
}

void Vehicle::update(const float deltaTime)
{
	updateMessages(deltaTime);

	m_forceMotion.update(deltaTime);

	// rotate the object based on its last & current position
	Vector2D diff = m_currentPosition - m_lastPosition;
	if (diff.Length() > 0) { // if zero then don't update rotation
		diff.Normalize();
		m_radianRotation = atan2f((float)diff.y, (float)diff.x); // this is used by DrawableGameObject to set the rotation
	}
	m_lastPosition = m_currentPosition;

	// set the current poistion for the drawablegameobject
	setPosition(m_currentPosition);

	DrawableGameObject::update(deltaTime);
}

// set the current position
void Vehicle::setPosition(Vector2D position)
{
	m_currentPosition = position;
	DrawableGameObject::setPosition(position);
}

void Vehicle::applyForceToPosition(const Vector2D& positionTo)
{
	// create a vector from the position to, and the current car position
	Vector2D posFrom = getPosition();
	m_force = positionTo - posFrom;
	Vector2D force = m_force;

	// normalise this (make it length 1)
	force.Normalize();

	getForceMotion()->applyForce(force);

	// Tutorial todo
	// create a message called 'SEEK' which detects when the car has reached a certain point
	// note: this has been done for you in the updateMessages function. 

	MessagePosition message = MessagePosition(MessageType::SEEK_MESSAGE, positionTo);
	addMessage(message);
}

/// <summary>calculates a precentage depending on how far from the destination we are, and applies that to a normalised force that would cancel out the cars current velocity</summary>
/// <param name="distance">The distance from the destination</param>
void Vehicle::brake(Vector2D toDestination)
{
	float distance = toDestination.Length();
	if (distance < BRAKE_DISTANCE)	//If we're within the destination area...
	{
		Vector2D force = m_force;	//Get the current force and apply it
		force.Normalize();
		m_forceMotion.applyForce(force);

		force *= -1;	//Get the inverse direction of the force
		float brakePercentage = max(0.0f, ((BRAKE_DISTANCE) - distance) / (BRAKE_DISTANCE));	//Calculate a percentage of how much of the brake area has been covered
		Vector2D brakeForce = force * brakePercentage;	//Apply a brake force according to how close the car is to the location													
		m_forceMotion.accummulateForce(brakeForce);

		OutputDebugStringA((std::to_string(brakePercentage) + "%\n").c_str());
		//OutputDebugStringA(("x = " + std::to_string(brakeForce.x) + ", y = " + std::to_string(brakeForce.y) + "\n").c_str());
	}
}

void Vehicle::SetState(AIState* state)
{
	m_state = state;
}

void Vehicle::setWaypointManager(WaypointManager* wpm)
{
	m_waypointManager = wpm;
}


// -------------------------------------------------------------------------------
// a really rubbish messaging system.. there is clearly a better way to do this...


void Vehicle::addMessage(MessagePosition message)
{
	m_messages.push_back(message);
}

void Vehicle::updateMessages(const float deltaTime)
{
	// create an iterator to iterate the message list
	list<MessagePosition>::iterator messageIterator = m_messages.begin();

	// loop while the iterator is not at the end
	while (messageIterator != m_messages.end())
	{
		MessagePosition msg = *messageIterator;

		switch (msg.type)
		{
		case MessageType::SEEK_MESSAGE:
		{
			Vector2D differenceVector = getPosition() - msg.position;
			brake(differenceVector);
			// WARNING - when testing distances, make sure they are large enough to be detected. Ask a lecturer if you don't understand why. 10 *should* be about right
			if (differenceVector.Length() < msg.radius)
			{
				messageReceived(msg);

				// delete the message. This will also assign(increment) the iterator to be the next item in the list
				messageIterator = m_messages.erase(messageIterator);
				continue; // continue the next loop (we don't want to increment below as this will skip an item)
			}
		}
		default:
		{
			break;
		}
		}
		messageIterator++; // incremenet the iterator
	}

	
}

void Vehicle::messageReceived(MessagePosition message)
{
	//If the vehicle has recieved a seek message, the vehicle has reached it's destination.
	switch (message.type)
	{
	case MessageType::SEEK_MESSAGE:
	{
		MessagePosition messagePosition = (MessagePosition)message;
		//At the destination, so stop moving
		m_forceMotion.clearForce();
		//Inform the state that we've arrived, regardless of if it needs it.
		m_state->DeliverMessage(Message(HERE_MESSAGE));
	}
	default:
	{
		break;
	}
	}
}


