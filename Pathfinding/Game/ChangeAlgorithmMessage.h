#ifndef CHANGEALGORITHMMESSAGE_H
#define CHANGEALGORITHMMESSAGE_H

#include "GameMessage.h"

#include "Vector2D.h"

class ChangeAlgorithmMessage : public GameMessage
{
public:
	ChangeAlgorithmMessage(int type);
	~ChangeAlgorithmMessage();

	void process();

private:
	int mType;
};

#endif CHANGEALGORITHMMESSAGE