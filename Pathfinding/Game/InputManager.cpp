#include "InputManager.h"

#include "Game.h"
#include "GraphicsSystem.h"
#include "Vector2D.h"
#include "GameMessageManager.h"
#include "GameApp.h"

#include "ChangeAlgorithmMessage.h"
#include "PathToMessage.h"
#include "UpdatePathsMessage.h"

InputManager::InputManager()
{
	mDdown = false;
	mAdown = false;
	mM1down = false;
	mM2down = false;

	pGameApp = dynamic_cast<GameApp*>(gpGame);

	if (!init())
	{
		// something wasn't installed
	}
}

InputManager::~InputManager()
{

}

bool InputManager::init()
{
	if (!al_install_keyboard())
	{
		printf("Keyboard not installed!\n");
		return false;
	}

	if (!al_install_mouse())
	{
		printf("Mouse not installed!\n");
		return false;
	}

	return true;
}

void InputManager::Update()
{
	//get current keyboard state
	al_get_keyboard_state(&keyState);

	al_get_mouse_state(&mouseState);

	//if 'escape' key was down then exit the loop
	if (al_key_down(&keyState, ALLEGRO_KEY_ESCAPE))
	{
		std::cout << "would be quitting game" << std::endl;
	}

	//if 'd' key is pressed, switch current algorithm to Dijkstra
	if (al_key_down(&keyState, ALLEGRO_KEY_D) && !mDdown)
	{
		mDdown = true;
		GameMessage* pMessage = new ChangeAlgorithmMessage(pGameApp, 0, 255, 0, 0);
		pGameApp->getMessageManager()->addMessage(pMessage, 0);
	}

	//if 'a' key is pressed, switch current algorithm to A*
	if (al_key_down(&keyState, ALLEGRO_KEY_A) && !mAdown)
	{
		mAdown = true;
		GameMessage* pMessage = new ChangeAlgorithmMessage(pGameApp, 1, 0, 0, 255);
		pGameApp->getMessageManager()->addMessage(pMessage, 0);
	}

	// changes the start position to the mouse position
	if (al_mouse_button_down(&mouseState, 1) && !mM1down)
	{
		mM1down = true;
		pGameApp->setPos(Vector2D(mouseState.x, mouseState.y));

		GameMessage* pMessage = new UpdatePathsMessage();
		pGameApp->getMessageManager()->addMessage(pMessage, 0);
	}

	// changes the end position to the mouse position
	if (al_mouse_button_down(&mouseState, 2) && !mM2down)//left mouse click
	{
		mM2down = true;
		pGameApp->setLastPos(Vector2D(mouseState.x, mouseState.y));

		GameMessage* pMessage = new UpdatePathsMessage();
		pGameApp->getMessageManager()->addMessage(pMessage, 0);
	}

	// check to see if you're still pressing the button so you can't create or delete a unit by just holding the key
	if (!al_key_down(&keyState, ALLEGRO_KEY_D)) { mDdown = false; }
	if (!al_key_down(&keyState, ALLEGRO_KEY_A)) { mAdown = false; }
	if (!al_mouse_button_down(&mouseState, 1)) { mM1down = false; }
	if (!al_mouse_button_down(&mouseState, 2)) { mM2down = false; }
}