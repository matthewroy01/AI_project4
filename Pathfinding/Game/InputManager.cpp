#include "InputManager.h"

#include "Game.h"
#include "GraphicsSystem.h"
#include "Vector2D.h"
#include "GameMessageManager.h"

InputManager::InputManager()
{
	mDdown = false;
	mAdown = false;

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

	//if 'escape' key was down then exit the loop
	if (al_key_down(&keyState, ALLEGRO_KEY_ESCAPE))
	{
		std::cout << "would be quitting game" << std::endl;
	}

	//if 'd' key is pressed, switch current algorithm to Dijkstra
	if (al_key_down(&keyState, ALLEGRO_KEY_D))
	{
		std::cout << "switching to Dijkstra" << std::endl;
	}

	//if 'a' key is pressed, switch current algorithm to A*
	if (al_key_down(&keyState, ALLEGRO_KEY_A))
	{
		std::cout << "switching to A*" << std::endl;
	}

	// check to see if you're still pressing the button so you can't create or delete a unit by just holding the key
	if (!al_key_down(&keyState, ALLEGRO_KEY_D)) { mDdown = false; }
	if (!al_key_down(&keyState, ALLEGRO_KEY_A)) { mAdown = false; }
}