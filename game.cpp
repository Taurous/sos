#include "game.hpp"

constexpr int DEFAULT_WIND_WIDTH = 600;
constexpr int DEFAULT_WIND_HEIGHT = 800;
constexpr double TARGET_SPF = 1.0 / 60.0;
std::string DISPLAY_TITLE = "Spirit of Survival";

game::game() : display(nullptr), ev_queue(nullptr), L(nullptr)
{
	
}

bool game::initAllegro()
{
	if (!al_init())
	{
		std::cerr << "Failed to load Allegro!" << std::endl;
		return false;
	}

	input.init();

	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();

	al_install_audio();
	al_init_acodec_addon();
	al_reserve_samples(4);

	return true;
}

bool game::createDisplay(std::string name, int w, int h)
{
	al_set_new_window_title(name.c_str());
	display = al_create_display(w, h);

	if (!display)
	{
		std::cerr << "Failed to create display!" << std::endl;
		return false;
	}

	return true;
}

bool game::initLua()
{
	L = luaL_newstate();
	luaL_openlibs(L);

	luaL_dostring(L, "a = 12\nprint(a)");

	return true;
}

int game::init()
{
	if (!initAllegro()) return 1;
	if (!createDisplay(DISPLAY_TITLE, DEFAULT_WIND_WIDTH, DEFAULT_WIND_HEIGHT)) return 2;
	if (!initLua()) return 3;

	// Register devices to generate events
	ev_queue = al_create_event_queue();
	al_register_event_source(ev_queue, al_get_keyboard_event_source());
	al_register_event_source(ev_queue, al_get_mouse_event_source());
	al_register_event_source(ev_queue, al_get_display_event_source(display));

	return 0;
}

void game::run()
{
	// Begin game
	sm.pushState(std::make_unique<SplashState>(sm, input));

	unsigned int frame_count = 0;
	double fps_time = 0.0;
	double acc_time = 0.0f;
	int fps = 0.0;

	auto last_time = std::chrono::high_resolution_clock::now();
	while (sm.running())
	{
		ALLEGRO_EVENT ev;

		if (al_get_next_event(ev_queue, &ev))
		{
			input.getInput(ev);
			sm.handleEvents();

			if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			{
				//TODO: Signal to state that window is being closed, save stuff
				sm.quit();
			}
		}

		auto current_time = std::chrono::high_resolution_clock::now();
		auto delta_time = std::chrono::duration<double>(current_time - last_time).count();
		last_time = current_time;

		sm.update(delta_time);

		fps_time += delta_time;
		if (fps_time >= 0.5)
		{
			fps_time /= frame_count;
			fps = int(1.0 / fps_time);
			fps_time = 0.0;
			frame_count = 0;

			std::stringstream ss;
			ss << DISPLAY_TITLE << " FPS: " << fps;
			al_set_window_title(display, ss.str().c_str());
		}

		if (acc_time >= TARGET_SPF)
		{
			sm.draw(false);

			al_flip_display();

			frame_count++;
			acc_time -= TARGET_SPF;
			if (acc_time >= 0.05) std::cout << "Time is accumulating..." << std::endl;
		}

		acc_time += delta_time;

		sm.removeDeadStates();
	}
}

game::~game()
{
	delete L;

	al_destroy_event_queue(ev_queue);
	al_destroy_display(display);
}