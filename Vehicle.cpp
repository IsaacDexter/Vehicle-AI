#include "Vehicle.h"
#include "Tasking.h"
#include <functional>
#include "CollisionHelper.h"

//https://www.cars-data.com/en/ford-fiesta/curb-weight
#define VEHICLE_MASS 0.00005f
#define ENGINE_FORCE 108.0f
//https://www.rac.co.uk/drive/advice/learning-to-drive/stopping-distances/
//https://calculator.academy/braking-force-calculator/
#define BRAKE_FORCE 8100.0f

Vehicle::Vehicle() : m_forceMotion(VEHICLE_MASS, getPositionAddress())
{
	m_currentPosition = Vector2D(0, 0);
	m_lastPosition = Vector2D(0, 0);
	m_direction = Vector2D(0, 0);
	m_waypointManager = nullptr;
	m_pTaskManager = nullptr;
	m_whiskers.push_back(new Whisker());
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
	m_forceMotion.update(deltaTime);

	// rotate the object based on its last & current position
	Vector2D diff = m_currentPosition - m_lastPosition;
	if (diff.Length() > 0) { // if zero then don't update rotation
		diff.Normalize();
		m_radianRotation = atan2f((float)diff.y, (float)diff.x); // this is used by DrawableGameObject to set the rotation
	}
	m_lastPosition = m_currentPosition;
	m_direction = diff;
	for (Whisker* whisker : m_whiskers)
	{
		projectWhisker(whisker);
		if (whisker->intersectsBuilding(m_waypointManager))
		{
			OutputDebugStringA("Whisker collision with building!\n");
		}
		if (whisker->intersectsVehicle(m_waypointManager))
		{
			OutputDebugStringA("Whisker collision with Vehicle!\n");
		}
	}

	// set the current poistion for the drawablegameobject
	setPosition(m_currentPosition);

	DrawableGameObject::update(deltaTime);

	m_forceMotion.clearForce();
}

// set the current position
void Vehicle::setPosition(Vector2D position)
{
	m_currentPosition = position;
	DrawableGameObject::setPosition(position);
}

Vector2D Vehicle::getPredictedPosition(const float interval)
{
	Vector2D expectedDisplacement = (getVelocity() * interval);	//gets the speed in m/s, and finds it in m/"when", based of this frame time
	return expectedDisplacement + m_currentPosition;
}

Vector2D Vehicle::getRandomDirection()
{
	float randomFloat = -1.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.0f - -1.0f)));
	float theta = std::acos(randomFloat);	//Get a random angle by generating a random float between -1 and 1, and finding the arccos of it.
	Vector2D direction = Vec2DfromAngle(theta);
	return direction;
}

void Vehicle::setWaypointManager(WaypointManager* wpm)
{
	m_waypointManager = wpm;
}

#pragma region Tasks

#pragma region Actions

void Vehicle::applyForceToPosition(const Vector2D& destination)
{
	Vector2D toDestination = destination - getPosition();
	toDestination.Normalize();		//Get the direction to the destination
	Vector2D desiredForce = toDestination;	//Multiply it by the vehicle's speed

	Vector2D currentForce = m_forceMotion.getForce();

	Vector2D force = desiredForce - currentForce;

	getForceMotion()->accumulateForce(force);
}

void Vehicle::applyForceFromPosition(const Vector2D& destination)
{
	Vector2D fromDestination = getPosition() - destination;
	fromDestination.Normalize();		//Get the direction to the destination
	Vector2D desiredForce = fromDestination;	//Multiply it by the vehicle's speed

	Vector2D currentForce = m_forceMotion.getForce();

	Vector2D force = desiredForce - currentForce;

	getForceMotion()->accumulateForce(force);
}

Vector2D Vehicle::getWanderPosition(float interval, float radius)
{
	Vector2D wanderDirection = getRandomDirection();	//Get a random direction
	wanderDirection *= radius;	//Scale it by the wander radius
	Vector2D wanderPosition = getPredictedPosition(1.0f) + wanderDirection;
	return wanderPosition;
}

#pragma endregion

#pragma region Checks

bool Vehicle::isArrived(Vector2D destination, float destinationRadiusSquared, float brakingRadiusSquared)
{
	float distanceSquared = destination.DistanceSq(getPosition());
	bool braking = brake(distanceSquared, brakingRadiusSquared);
	bool arrived = false;
	if (braking)	//If we're braking, we must be roughly near the destination area
	{
		arrived = distanceSquared < destinationRadiusSquared;
		if (arrived)
		{
			//m_forceMotion.clearForce();
		}
	}
	return braking && arrived;
}

bool Vehicle::isPassed(Vector2D destination, float destinationRadiusSquared)
{
	float distanceSq = destination.DistanceSq(getPosition());
	bool arrived = arrived = distanceSq < destinationRadiusSquared;
	return arrived;
}

bool Vehicle::brake(float distanceSquared, float brakingRadiusSquared)
{
	bool inRange = distanceSquared < brakingRadiusSquared;
	if (inRange)	//If we're within the destination area...
	{
		Vector2D force = m_forceMotion.getForce();	//Get the current force and apply it
		force.Normalize();

		force *= -1;	//Get the inverse direction of the force
		float brakePercentage = max(0.0f, ((brakingRadiusSquared)-distanceSquared) / (brakingRadiusSquared));	//Calculate a percentage of how much of the brake area has been covered
		Vector2D brakeForce = force * (brakePercentage / 2);	//Apply a brake force according to how close the car is to the location													
		m_forceMotion.accumulateForce(brakeForce);
	}
	return inRange;
}

bool Vehicle::brake(Vector2D destination, float brakingRadiusSquared)
{
	Vector2D toDestination = destination - getPosition();	//Get the vector to the destination
	float distanceSquared = toDestination.LengthSq();		//Get the length squared of that vector to get the distance squared
	bool inRange = distanceSquared < brakingRadiusSquared;
	if (inRange)	//If we're within the destination area...
	{
		toDestination.Normalize();	//Find the direction away from the destination
		toDestination *= -1;

		float brakePercentage = max(0.0f, ((brakingRadiusSquared)-distanceSquared) / (brakingRadiusSquared));	//Calculate a percentage of how much of the brake area has been covered
		Vector2D brakeForce = toDestination * (brakePercentage / 2);	//Apply a brake force in the opposite direction  to the destination according to how close the car is to the location													
		m_forceMotion.accumulateForce(brakeForce);
	}
	return inRange;
}

void Vehicle::projectWhisker(Whisker* outWhisker)
{
	Vector2D direction = getForceMotion()->getVelocity();	//get the vehicles direction from velocity
	Vector2D position = getPosition() + direction;			//Find the position the whisker ends at
															//Check to see if a line between the car and that position would intersect
	outWhisker->SetLine(getPosition(), position);
}

void Vehicle::projectWhisker(Whisker* outWhisker, float theta)
{
	Vector2D direction = getForceMotion()->getVelocity();	//get the vehicles direction from velocity
	Vector2D angleDirection = Vec2DfromAngle(theta);		//Get the direction of the angle
	direction *= angleDirection;							//Combine with with the vehicles current angle to project the angle from the vehicles direction
	Vector2D position = getPosition() + direction;			//Find the position the whisker ends at
															//Check to see if a line between the car and that position would intersect
	outWhisker->SetLine(getPosition(), position);
}

void Vehicle::projectWhisker(Whisker* outWhisker, float theta, float distance)
{
	Vector2D direction = getDirection();					//get the vehicles direction from velocity
	direction *= distance;									//Multiply it by the distance to scale it 
	Vector2D angleDirection = Vec2DfromAngle(theta);		//Get the direction of the angle
	direction *= angleDirection;							//Combine with with the vehicles current angle to project the angle from the vehicles direction
	Vector2D position = getPosition() + direction;			//Find the position the whisker ends at
															//Check to see if a line between the car and that position would intersect
	outWhisker->SetLine(getPosition(), position);
}

#pragma endregion

#pragma region SteeringBehaviours

void Vehicle::Wander()
{
	Vector2D destination = getWanderPosition();	//get a random wander position;

	executeFunc execute = [this] {; };	//Head towards that position
	maintainFunc maintain = [this, destination](float dt) { applyForceToPosition(destination); };
	completeFunc complete = [this] { this->Wander(); };									//Once we're there, wander again, queueing up a new task at a new position
	checkFunc check = [this, destination] { return this->isPassed(destination); };		//Each frame this task is active, check if we've passed the position

	m_pTaskManager->AddTask(new Task(execute, maintain, complete, check));
}

void Vehicle::Ramble()
{
	Vector2D destination = m_waypointManager->getRandomWaypoint()->getPosition();		//Get a random waypoint's position

	executeFunc execute = [this] {; };	//Head towards that position
	maintainFunc maintain = [this, destination](float dt) { applyForceToPosition(destination); };
	completeFunc complete = [this] { this->Ramble(); };									//Once we're there, wander again, queueing up a new task at a new position
	checkFunc check = [this, destination] { return this->isPassed(destination); };		//Each frame this task is active, check if we've passed the position

	m_pTaskManager->AddTask(new Task(execute, maintain, complete, check));
}

void Vehicle::Seek(DrawableGameObject* soughtObject)
{
	executeFunc execute = [this] {; };
	maintainFunc maintain = [this, soughtObject](float dt) { Vector2D destination = soughtObject->getPosition(); applyForceToPosition(destination); };	//each frame, get it's current position once more and seek to it.
	completeFunc complete = [this] {; };
	checkFunc check = [this, soughtObject] { this->isPassed(soughtObject->getPosition()); return false; };			//Each frame this task is active, check if we've passed the position

	m_pTaskManager->AddTask(new Task(execute, maintain, complete, check));
}

void Vehicle::Seek(Vector2D destination)
{
	executeFunc execute = [this] {; };	//Head towards that position
	maintainFunc maintain = [this, destination](float dt) { applyForceToPosition(destination); };
	completeFunc complete = [this] {; };												//Once we're there, wander again, queueing up a new task at a new position
	checkFunc check = [this, destination] { return this->isPassed(destination); };		//Each frame this task is active, check if we've passed the position

	m_pTaskManager->AddTask(new Task(execute, maintain, complete, check));
}

void Vehicle::Arrive(DrawableGameObject* soughtObject)
{
	executeFunc execute = [this] {; };
	maintainFunc maintain = [this, soughtObject](float dt) { Vector2D destination = soughtObject->getPosition(); applyForceToPosition(destination); };	//each frame, get it's current position once more and seek to it.
	completeFunc complete = [this] {; };
	checkFunc check = [this, soughtObject] { return this->isArrived(soughtObject->getPosition()); };			//Each frame this task is active, check if we've passed the position

	m_pTaskManager->AddTask(new Task(execute, maintain, complete, check));
}

void Vehicle::Arrive(Vector2D destination)
{
	executeFunc execute = [this] {; };	//Head towards that position
	maintainFunc maintain = [this, destination](float dt) { applyForceToPosition(destination); };
	completeFunc complete = [this] {; };												//Once we're there, wander again, queueing up a new task at a new position
	checkFunc check = [this, destination] { return this->isArrived(destination); };		//Each frame this task is active, check if we've passed the position

	m_pTaskManager->AddTask(new Task(execute, maintain, complete, check));
}

void Vehicle::Flee(DrawableGameObject* soughtObject)
{
	executeFunc execute = [this] {; };
	maintainFunc maintain = [this, soughtObject](float dt) { Vector2D destination = soughtObject->getPosition(); applyForceFromPosition(destination); };	//each frame, get it's current position once more and seek to it.
	completeFunc complete = [this] {; };
	checkFunc check = [this, soughtObject] { return !this->isPassed(soughtObject->getPosition(), 500000.0f); };	//Each frame this task is active, check if we're not within range

	m_pTaskManager->AddTask(new Task(execute, maintain, complete, check));
}

void Vehicle::Flee(Vector2D destination)
{
	executeFunc execute = [this] {; };	//Head towards that position
	maintainFunc maintain = [this, destination](float dt) { applyForceFromPosition(destination); };
	completeFunc complete = [this] {; };												//Once we're there, wander again, queueing up a new task at a new position
	checkFunc check = [this, destination] { return !this->isPassed(destination, 500000.0f); };	//Each frame this task is active, check if we're not within range

	m_pTaskManager->AddTask(new Task(execute, maintain, complete, check));
}

void Vehicle::Evade(Vehicle* soughtObject)
{
	executeFunc execute = [this] {; };
	maintainFunc maintain = [this, soughtObject](float dt) { applyForceFromPosition(soughtObject->getPredictedPosition(1.0f)); };
	completeFunc complete = [this] {; };
	checkFunc check = [this, soughtObject] { return !this->isPassed(soughtObject->getPosition(), 500000.0f); };	//Each frame this task is active, check if we're not within range

	m_pTaskManager->AddTask(new Task(execute, maintain, complete, check));
}

void Vehicle::Intercept(Vehicle* soughtObject)
{
	executeFunc execute = [this] {; };
	maintainFunc maintain = [this, soughtObject](float dt) { applyForceToPosition(soughtObject->getPredictedPosition(0.25f)); };	//Seek to where it'll be in a quarter second
	completeFunc complete = [this] {; };
	checkFunc check = [this, soughtObject] { this->isArrived(soughtObject->getPosition()); return false; };	//Each frame this task is active, check if we're not within range

	m_pTaskManager->AddTask(new Task(execute, maintain, complete, check));
}

#pragma endregion

#pragma endregion