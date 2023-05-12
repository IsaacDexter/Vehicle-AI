#pragma once

#include "DrawableGameObject.h"
#include "WaypointManager.h"
#include "PickupManager.h"
#include "Waypoint.h"
#include "Vector2D.h"
#include "Collidable.h"
#include "ForceMotion.h"
#include <string>
#include <map>
#include "Whisker.h"
#include "SM.h"


#define VEHICLE_MASS 0.00005f

class TaskManager;
struct Task;

struct Passenger;

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

#pragma region Movement



	void setPosition(Vector2D position); // the current position
	Vector2D* getPositionAddress() { return &m_currentPosition; }
	Vector2D getVelocity() { return m_forceMotion.getVelocity(); }
	/// <returns>The vehicles normalized direction, calculated from the change in position.</returns>
	Vector2D getDirection() { return m_direction; }
	Vector2D getScale() { return Vector2D(m_scale.x, m_scale.y); };

	/// <param name="interval">How far ahead, in seconds, to predict the objects position. Passing in delta will get the objects expected position in 1 frame's time, for example</param>
	/// <returns>The expected position that the object will be in "interval" seconds time.</returns>
	Vector2D getPredictedPosition(const float interval);
	/// <param name="interval">How far behind, in seconds, to predict the objects position. Passing in delta will get the objects expected position from 1 frame ago, for example</param>
	/// <returns>The expected position that the object was in "interval" seconds ago.</returns>
	Vector2D getPreviousPosition(const float interval);
	/// <returns>A random unit vector.</returns>
	Vector2D getRandomDirection();


	void setWaypointManager(WaypointManager* wpm);
	WaypointManager* getWaypointManager() const { return m_waypointManager; };
	void setTaskManager(TaskManager* taskManager) { m_pTaskManager = taskManager; };
	void setStateManager(SM* stateManager) { m_pStateManager = stateManager; };
	void setState(State* state) { if (m_pStateManager != nullptr) m_pStateManager->SetState(state); };
	void hasCollided() {}

	void SetPickupManager(PickupManager* pickupManager) { m_pickupManager = pickupManager; };
	PickupManager* GetPickupManager() const { return m_pickupManager; };

	ForceMotion* getForceMotion() { return &m_forceMotion; }

	std::vector<Whisker*> getWhiskers() { return m_whiskers; };

#pragma endregion

#pragma region DecisionMaking

protected:
	/// <summary>The vehicles speed, in an arbitary unit, increased by speed pickups and reduced by a lack of fuel.</summary>
	float m_speed = 1.0f;
	/// <summary>The speed to move at without fuel.</summary>
	const float m_emptySpeed = 0.2f;
	/// <summary>Stores the last amount of fuel.</summary>
	float* m_lastSpeed = nullptr;

	/// <summary>How many units of fuel to consume each second, increased by picking up passengers and restored by delivering them.</summary>
	float m_fuelConsumption = 1.0f;
	/// <summary>The total amount of fuel that can be fit in the tank.</summary>
	const float m_maxFuel = 30.0f;
	float m_fuel = m_maxFuel;

	//The amount of money, in units, paid to the taxi by its fares.
	float m_money = 0.0f;

	/// <summary>The maximum amount of fares to be picked up in m_fares.</summary>
	const unsigned int m_maxFares = 4;
	/// <summary>The map of destinations to passengers to be delivered. Constrained in size by m_maxFares.</summary>
	std::vector<Passenger*> m_fares;

public:
	void GiveTip(float tip) { m_money += tip; OutputDebugStringA(("m_money == " + std::to_string(m_money)).c_str()); };

	float GetFuelConsumption() const { return m_fuelConsumption; };
	void SetFuelConsumption(float fuelConsumption) { m_fuelConsumption = fuelConsumption; };
	void ConsumeFuel(float dt);
	void Refuel(float fuel);
	float GetFuel() const { return m_fuel; };
	
	float GetSpeed() const { return m_speed; };
	void SetSpeed(float speed) { m_speed = speed; };

	void CheckFares();
	Passenger* GetNearestFare();
	Passenger* GetNearestFare(float& distanceSq_out);
	int GetPassengerCount() const { return m_fares.size(); };

#pragma region Pickups

public:

	void CollectPickup(PickupItem* pickup);
	/// <summary><para>Tryies to collect a passenger and increases the vehicles fuel consumption as a result, until the passenger is delivered.</para>
	/// <para>Upon delivering a passenger, the fuel consumption rate will restore and the space will free up.</para>
	/// <para>A passenger will take up a space in the passengers array. Without space for another passenger, no passenger will be collected.</para>
	/// <para>A vehicle will prioritise picking up passengers when they have a comfortable amount of fuel, and space, and aren't too close too a destination.</para></summary>
	/// <param name="passenger">The pickup to store as the handler to the character, and to call pickup upon.</param>
	/// <param name="destination">The destination asscociated with the character.</param>
	bool PickupPassenger(Passenger* passenger);
	/// <summary><para>When a destination has been reached, this is called.</para>
	/// <para>It removes the passenger from the array, freeing up a little bit of space and restoring fuel consumption.</para></summary>
	/// <param name="destination">The destination to use as the key to find which character to remove.</param>
	void DeliverPassenger(Vector2D destination);

#pragma endregion

#pragma endregion

#pragma region Tasks

#pragma region Actions

	/// <summary>Applies a force according to the engines BHP or whatever that value is. Either way, it scales.</summary>
	/// <param name="direction">The direction, normalized, to apply the force in.</param>
	void applyForceInDirection(const Vector2D& direction);

	/// <summary>Applies the force regardless of scale</summary>
	/// <param name="force">The force, unnormalised, to apply</param>
	void applyForce(const Vector2D& force);

	void applyForceToPosition(const Vector2D& positionTo);

	void applyForceFromPosition(const Vector2D& positionTo);

	void avoidVehicles();

	void avoidBuildings();

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
	bool isArrived(Vector2D destination, float destinationRadiusSquared = 576.0f, float brakingRadiusSquared = 4096.0f);
	/// <summary>Checks to see if we have reached a destination, if we have, don't stop moving, and return true.</summary>
	/// <param name="destination">The Vector2D of the position we want to reach.</param>
	/// <param name="destinationRadiusSquared">The radius, squared, of the zone we consider part of the destination. Somewhat like a margin of error.</param>
	/// <returns>Whether or not we have reached, but not stopped at, our destination.</returns>
	bool isPassed(Vector2D destination, float destinationRadiusSquared = 576.0f);
	/// <summary>Checks to see if we're within braking distance of a destination. If we are, start slowing as we reach the destination.</summary>
	/// <param name="distanceSquared">The distance, squared, between the vehice and the destination.</param>
	/// <param name="brakingRadiusSquared">The radius, squared, of the zone in which we want to start braking.</param>
	/// <returns>Whether or not we are within braking distance or a destination, not if we've come to a stop.</returns>
	bool brake(float distanceSquared, float brakingRadiusSquared = 4096.0f);
	/// <summary>Checks to see if we're within braking distance of a destination. If we are, start slowing as we reach the destination.</summary>
	/// <param name="destination">The destination to centre the braking zone around.</param>
	/// <param name="brakingRadiusSquared">The radius, squared, of the zone in which we want to start braking.</param>
	/// <returns>Whether or not we are within braking distance or a destination, not if we've come to a stop.</returns>
	bool brake(Vector2D destination, float brakingRadiusSquared = 4096.0f);
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
	Task* Wander();

	/// <summary>Get's a random waypoint in the area and adds a task that will apply force towards that position each frame, and on completion will call Wander again.</summary>
	Task* Ramble();

	/// <summary>The Seek steering behavior returns a force that directs an agent toward a target game object's position.
	///<para>We calculate the vector to the target position, Calculate the unit equivalent, And scale it to the maximum speed of the agent.</para></summary>
	/// <param name="soughtObject">The game object whose position will be continually sought.</param>
	Task* Seek(DrawableGameObject* soughtObject);

	/// <summary>The Seek steering behavior returns a force that directs an agent toward a target position.
	///<para>We calculate the vector to the target position, Calculate the unit equivalent, And scale it to the maximum speed of the agent.</para></summary>
	/// <param name="position">The position to be sought.</param>
	Task* Seek(Vector2D position);

	/// <summary>The Seek steering behavior returns a force that directs an agent toward a target position, and stops when it reaches that position.
	///<para>We calculate the vector to the target position, Calculate the unit equivalent, And scale it to the maximum speed of the agent.</para></summary>
	/// <param name="position">The position to be sought.</param>
	Task* Arrive(Vector2D position);

	/// <summary>The Seek steering behavior returns a force that directs an agent toward a target game object's position, and stops when it reaches that position.
	///<para>We calculate the vector to the target position, Calculate the unit equivalent, And scale it to the maximum speed of the agent.</para></summary>
	/// <param name="soughtObject">The game object whose position will be continually sought.</param>
	Task* Arrive(DrawableGameObject* soughtObject);

	/// <summary>The Flee steering behavior returns a force that directs an agent away from a target game object's position.
	///<para>We calculate the vector from the target position, Calculate the unit equivalent, And scale it to the maximum speed of the agent.</para></summary>
	/// <param name="soughtObject">The game object whose position will be continually fled, while in range.</param>
	Task* Flee(DrawableGameObject* soughtObject);

	/// <summary>The Seek steering behavior returns a force that directs an agent away from a target position.
	///<para>We calculate the vector from the target position, Calculate the unit equivalent, And scale it to the maximum speed of the agent.</para></summary>
	/// <param name="position">The position to be fled, while in range.</param>
	Task* Flee(Vector2D position);

	/// <summary>The Evade steering behavior returns a force that directs an agent away from a target game object's predicted position.
	///<para>We calculate the vector from the target position's predicted position, Calculate the unit equivalent, And scale it to the maximum speed of the agent.</para></summary>
	/// <param name="soughtObject">The game object whose position will be continually evaded, while in range.</param>
	Task* Evade(Vehicle* soughtObject);

	/// <summary>The Intercept steering behavior returns a force that directs an agent away towards a target game object's predicted position.
	///<para>We calculate the vector to the target position's predicted position, Calculate the unit equivalent, And scale it to the maximum speed of the agent.</para></summary>
	/// <param name="soughtObject">The game object whose position will be continually intercepted.</param>
	Task* Intercept(Vehicle* soughtObject);

	/// <summary>The car seeks, if it gets close to the other car, it will go round it, but continue moving towards its destination.</summary>
	/// <param name="position">The position to seek to.</param>
	Task* ObstaceAvoidance(Vector2D position);
	
	/// <summary>Adds a task that causes the vehicle to start avoiding buildings.</summary>
	Task* BuildingAvoidance();

	/// <summary>this car finds and follows the target car, always staying behind it.</summary>
	/// <param name="soughtObject">The car to pursue after</param>
	Task* Pursuit(Vehicle* soughtObject);

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
	PickupManager* m_pickupManager;
	SM* m_pStateManager;
	ForceMotion m_forceMotion;

	/// <summary>How much to multiply forces that affect vehicles by. represents an engines bhp</summary>
	const float m_forceMultiplier = 1.0f;

	std::vector<Whisker*> m_whiskers;
};

