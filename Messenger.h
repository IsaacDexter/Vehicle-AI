#pragma once
#include "Vector2D.h"

enum MessageType
{
	NONE_MESSAGE = 0,
	SEEK_MESSAGE,
	GOTO_MESSAGE,
	HERE_MESSAGE
};

struct Message
{
	MessageType type;
	Message(MessageType type)
	{
		this->type = type;
	}
	Message()
	{
		type = NONE_MESSAGE;
	}
};

struct MessagePosition : Message
{
	
	/// <summary>The position to go to.</summary>
	Vector2D position;
	/// <summary>The margin of error</summary>
	float radius;
	MessagePosition(MessageType type, Vector2D position) : Message()
	{
		this->position = position;
		this->type = type;
		this->radius = 100;
	}
	MessagePosition(Message _message) 
	{
		this->type = _message.type;
		this->radius = 100;
	}
};