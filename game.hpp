#pragma once

#include <iostream> // For std::cerr
#include <chrono>	// For FPS counting and providing game with tick time
#include <fstream>
#include <sstream>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include <lua.hpp>

#include "input.hpp"
#include "state_machine.hpp"
#include "splash_state.hpp"

class game
{
public:
	game();
	~game();

	int init();
	void run();

private:
	bool initAllegro();
	bool createDisplay(std::string name, int w, int h);
	bool initLua();

	ALLEGRO_DISPLAY* display;
	ALLEGRO_EVENT_QUEUE* ev_queue;

	InputHandler input;
	StateMachine sm;

	lua_State* L;
};

