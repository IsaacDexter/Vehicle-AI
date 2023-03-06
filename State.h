#pragma once

class Vehicle;
class Message;

class AIState
{
public:
	AIState() { ; };

	virtual void Enter() = 0;
	virtual AIState* Update(Vehicle* agent, float dt) = 0;

	virtual void DeliverMessage(Message message) = 0;
protected:
	virtual AIState* CheckTransition(Vehicle* agent) = 0;
	virtual void Exit() = 0;
};