#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "Trackable.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include <sstream>
#include <stdio.h>

class Game;
class Vector2D;
class GameApp;

class InputManager : public Trackable
{
private:
	// bools that keep track of whether or not a key is still being pressed (sort of a way to cheat in "al_key_up")
	bool mAdown, mDdown;

	// mouse and keyboard state
	ALLEGRO_MOUSE_STATE mouseState;
	ALLEGRO_KEYBOARD_STATE keyState;

	GameApp* pGameApp;

public:
	InputManager();
	~InputManager();

	bool init();

	void Update();
};

#endif