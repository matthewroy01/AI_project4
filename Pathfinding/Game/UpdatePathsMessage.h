#pragma once

#include "GameMessage.h"

#include "Vector2D.h"

class UpdatePathsMessage :public GameMessage
{
public:
	UpdatePathsMessage();
	~UpdatePathsMessage();

	void process();

private:
	
};