#include "state_machine.hpp"

StateMachine::StateMachine() : m_running(true), m_res(false) {}

StateMachine::~StateMachine()
{
	removeDeadStates();
	while (!m_states.empty()) {
		m_states.pop_back();
	}
}

void StateMachine::changeState(std::unique_ptr<AbstractState> state)
{
	popState();
	pushState(std::move(state));
}

void StateMachine::pushState(std::unique_ptr<AbstractState> state)
{
	if (!m_states.empty()) {
		m_states.back().get()->pause();
	}

	m_states.push_back(std::move(state));
}

void StateMachine::popState()
{
	if (!m_states.empty()) {
		m_dead_states.push_back(std::move(m_states.back()));
		m_states.pop_back();
		if (!m_states.empty()) m_states.back().get()->resume();
	}
}

void StateMachine::handleEvents()
{
	m_states.back()->handleEvents();
}

void StateMachine::update(double deltaTime)
{
	m_states.back()->update(deltaTime);
}

void StateMachine::draw(bool draw_all)
{
	if (draw_all)
	{
		for (auto const &ss : m_states)
		{
			ss.get()->draw();
		}
	}
	else m_states.back()->draw();
}

void StateMachine::removeDeadStates()
{
	while (!m_dead_states.empty())
	{
		m_dead_states.pop_back();
	}
}