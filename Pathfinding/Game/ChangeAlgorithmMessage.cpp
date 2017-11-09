#include "ChangeAlgorithmMessage.h"

ChangeAlgorithmMessage::ChangeAlgorithmMessage(int type)
	:GameMessage(CHANGE_ALGORITHM_MESSAGE)
{
	mType = type;
}

ChangeAlgorithmMessage::~ChangeAlgorithmMessage()
{
}

void ChangeAlgorithmMessage::process()
{
	switch (mType)
	{
		case 0:
		{
			std::cout << "ChangeAlgorithmMessage: switching to Dijkstra" << std::endl;
		}
		case 1:
		{
			std::cout << "ChangeAlgorithmMessage: switching to A*" << std::endl;
		}
		default:
		{
			std::cout << "ChangeAlgorithmMessage: " << mType << " is not a valid type" << std::endl;
		}
	}
}