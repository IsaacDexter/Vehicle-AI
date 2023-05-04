#pragma once

class State;

/// <summary>Abstract class that finite, stack based and fuzzy state machines inherit from</summary>
class SM abstract
{
public:
	SM() { ; };
	virtual void Update(float dt) = 0;
	virtual void SetState(State* state) = 0;
	virtual bool HasState() = 0;

};