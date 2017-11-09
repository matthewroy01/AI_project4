#include "ChangeAlgorithmMessage.h"
#include "GameApp.h"
#include "GridPathfinder.h"

ChangeAlgorithmMessage::ChangeAlgorithmMessage(GameApp* app, int type, int r, int g, int b)
	:GameMessage(CHANGE_ALGORITHM_MESSAGE)
{
	mApp = app;
	mType = type;
	mR = r;
	mG = g;
	mB = b;
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
			mApp->changeAlgorithm(0);
			mApp->getPathfinder()->setCurrentColor(mR, mG, mB);
			break;
		}
		case 1:
		{
			std::cout << "ChangeAlgorithmMessage: switching to A*" << std::endl;
			mApp->changeAlgorithm(1);
			mApp->getPathfinder()->setCurrentColor(mR, mG, mB);
			break;
		}
		default:
		{
			std::cout << "ChangeAlgorithmMessage: " << mType << " is not a valid type" << std::endl;
		}
	}
}