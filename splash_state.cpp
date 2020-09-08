#include <iostream>

#include "splash_state.hpp"

#include "input.hpp"
#include "state_machine.hpp"

constexpr float total_time = 4.f;
constexpr float fade_in_time = 0.5f;
constexpr float fade_duration = 0.15f;
constexpr float fade_out_time = 3.6f;

SplashState::SplashState(StateMachine& state_machine, InputHandler& input)
	: AbstractState(state_machine, input), acc_time(0)
{
	
}

SplashState::~SplashState()
{
	
}

void SplashState::pause()
{
}

void SplashState::resume()
{
}

void SplashState::handleEvents()
{
	if (m_input.isMousePressed(MOUSE::LEFT))
	{
		acc_time = total_time;
	}
}

void SplashState::update(double deltaTime)
{
	acc_time += deltaTime;
	if (acc_time >= total_time)
	{
		m_states.quit();
		//m_states.changeState(std::make_unique<PlayState>(m_states, m_input));
	}
}

void SplashState::draw()
{
	
}
