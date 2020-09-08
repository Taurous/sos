#pragma once

#include <allegro5/allegro.h>
#include "abstract_state.hpp"

class SplashState : public AbstractState
{
public:
	SplashState(StateMachine& state_machine, InputHandler& input);

	~SplashState() override;

	void pause() override;
	void resume() override;

	void handleEvents() override;
	void update(double deltaTime) override;
	void draw() override;

private:
	double acc_time;
};