#include "Vehicle.h"

#define VEHICLE_MASS 0.00005f
#define BRAKE_DISTANCE 100.0f	//The distance required to slow to a stop

#define SEEK_RADIUS 10

Vehicle::Vehicle() : m_forceMotion(VEHICLE_MASS, getPositionAddress())
{
	m_currentPosition = Vector2D(0,0);
	m_lastPosition = Vector2D(0, 0);
	m_waypointManager = nullptr;
	m_tasks = std::deque<std::function<void()>>();

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

	if (!m_tasks.empty())
	{
		m_tasks.front()();
		m_tasks.pop_front();
	}

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

void Vehicle::applyForceToPosition(const Vector2D& positionTo, MessageType type)
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

	Message message;
	message.type = type;
	message.position = positionTo;
	addMessage(message);
}

/// <summary>calculates a precentage depending on how far from the destination we are, and applies that to a normalised force that would cancel out the cars current velocity</summary>
/// <param name="distance">The distance from the destination</param>
void Vehicle::brake(Vector2D toDestination)
{
	float distance = toDestination.Length();
	if (distance < BRAKE_DISTANCE - SEEK_RADIUS)	//If we're within the destination area...
	{
		Vector2D force = m_force;	//Get the current force and apply it
		force.Normalize();
		m_forceMotion.applyForce(force);

		force *= -1;	//Get the inverse direction of the force
		float brakePercentage = max(0.0f, ((BRAKE_DISTANCE - SEEK_RADIUS) - distance) / (BRAKE_DISTANCE - SEEK_RADIUS));	//Calculate a percentage of how much of the brake area has been covered
		Vector2D brakeForce = force * brakePercentage;	//Apply a brake force according to how close the car is to the location													
		m_forceMotion.accummulateForce(brakeForce);

		OutputDebugStringA((std::to_string(brakePercentage) + "%\n").c_str());
		//OutputDebugStringA(("x = " + std::to_string(brakeForce.x) + ", y = " + std::to_string(brakeForce.y) + "\n").c_str());
	}
}

void Vehicle::setWaypointManager(WaypointManager* wpm)
{
	m_waypointManager = wpm;
}


#pragma region SteeringBehaviours

void Vehicle::Wander()
{
	applyForceToPosition(m_waypointManager->getRandomWaypoint()->getPosition(), WANDER_MESSAGE);  //Get a random waypoint and set the car to seek to that position.
}

void Vehicle::Seek(DrawableGameObject* soughtObject)
{
	Seek(soughtObject->getPosition());
}

void Vehicle::Seek(Vector2D position)
{
	//Vector2D desiredVelocity = position - m_currentPosition;	//Calculates a vector towards the target position
	//desiredVelocity.Normalize();								//Calculates the unit equivalent
	//															//Scales it to the maximum speed of the agent
	//Vector2D currentVelocity = m_forceMotion.getVelocity();		//Subtracts current velocity... 
	//Vector2D seekForce = desiredVelocity - currentVelocity;		//...from desired velocity to get the seek force, which in turn allows the agent to turn
	//m_forceMotion.accummulateForce(seekForce);					//Apply said force

	//addMessage(Message(MessageType::SEEK_MESSAGE, position));

	applyForceToPosition(position, SEEK_MESSAGE);
}



#pragma endregion




// -------------------------------------------------------------------------------
// a really rubbish messaging system.. there is clearly a better way to do this...


void Vehicle::addMessage(Message message)
{
	m_messages.push_back(message);
}

void Vehicle::updateMessages(const float deltaTime)
{
	// create an iterator to iterate the message list
	list<Message>::iterator messageIterator = m_messages.begin();

	// loop while the iterator is not at the end
	while (messageIterator != m_messages.end())
	{
		Message message = *messageIterator;
		switch (message.type)
		{
		case MessageType::ARRIVE_MESSAGE:
		{
			Vector2D differenceVector = getPosition() - message.position;
			brake(differenceVector);
			// WARNING - when testing distances, make sure they are large enough to be detected. Ask a lecturer if you don't understand why. 10 *should* be about right
			if (differenceVector.Length() < SEEK_RADIUS)
			{
				messageReceived(message);

				// delete the message. This will also assign(increment) the iterator to be the next item in the list
				messageIterator = m_messages.erase(messageIterator);
				continue; // continue the next loop (we don't want to increment below as this will skip an item)
			}
			break;
		}
		case MessageType::WANDER_MESSAGE:
		{
			Vector2D differenceVector = getPosition() - message.position;
			// WARNING - when testing distances, make sure they are large enough to be detected. Ask a lecturer if you don't understand why. 10 *should* be about right
			if (differenceVector.Length() < SEEK_RADIUS)
			{
				
			}
			break;
		}
		case MessageType::SEEK_MESSAGE:
		{
			//Receiving a seek message could mean we're heading towards a position, or we could be seeking an specific moving game object
			//Either way, we need to seek again to update our velocity to provide a proper turn, as set out in the lecture.
			messageReceived(message);

			// delete the message. This will also assign(increment) the iterator to be the next item in the list
			messageIterator = m_messages.erase(messageIterator);
			continue; // continue the next loop (we don't want to increment below as this will skip an item)
			break;
		}
		default:
			break;
		}
		messageIterator++; // incremenet the iterator
	}

	
}

void Vehicle::messageReceived(Message message)
{
	//If the vehicle has recieved a seek message, the vehicle has reached it's destination.
	switch (message.type)
	{
	case MessageType::ARRIVE_MESSAGE:
	{
		//At the destination, so stop moving
		m_forceMotion.clearForce();
		break; 
	}
	case MessageType::WANDER_MESSAGE:
	{
		//At the destination, so don't stop moving and find a new destination to move to.
		Wander();
		break;
	}
	case MessageType::SEEK_MESSAGE:
	{
		void (Vehicle:: * task)(Vector2D);	//Declare function pointer to a vehicle method that takes a Vector2D as a parameter. 
											//Note the Vehicle:: to denote that this is a member function pointer.
		task = &Vehicle::Seek;				// use the :: syntax

		Vehicle* v = this;
		//(this->*task)(message.position);	//Call the seek behaviour with the correct syntax

		//std::function<void()> lambda = [](const std::string& s) { return std::stoi(s); };
		m_tasks.push_back([this] { this->Seek(Vector2D(100, 0)); OutputDebugStringA("Popped and called task!");  });	//Call the seek behaviour with the correct syntax

		break;
	}
	default:
		break;
	}
}


