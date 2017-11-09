#ifndef CHANGEALGORITHMMESSAGE_H
#define CHANGEALGORITHMMESSAGE_H

#include "GameMessage.h"

#include "Vector2D.h"

class GameApp;

class ChangeAlgorithmMessage : public GameMessage
{
public:
	ChangeAlgorithmMessage(GameApp* app, int type, int r, int g, int b);
	~ChangeAlgorithmMessage();

	void process();

private:
	GameApp* mApp;
	int mType, mR, mG, mB;
};

#endif CHANGEALGORITHMMESSAGE