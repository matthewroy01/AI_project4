#include "UpdatePathsMessage.h"
#include "GameApp.h"

UpdatePathsMessage::UpdatePathsMessage()
	:GameMessage(UPDATE_PATHS_MESSAGE)
{
}

UpdatePathsMessage::~UpdatePathsMessage()
{
}

void UpdatePathsMessage::process()
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	pGame->UpdatePaths();
}