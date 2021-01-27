#include "StateManager.hpp"

void StateManager::add_state(GameState state, BaseState& state_instance)
{
	states.insert({ state, std::reference_wrapper(state_instance) });
}

void StateManager::push(GameState state)
{
	if (!stack.empty()) {
		stack.back().get().on_bury();
	}
	stack.push_back(states.at(state));
	stack.back().get().on_entrance(renderer);
}

void StateManager::pop(unsigned int n)
{
	while (n-- > 0) {
		stack.pop_back();
	}
}

void StateManager::handle_input(SDL_Event& event)
{
	stack.back().get().handle_input(event);
}

void StateManager::draw_scene(uint32_t dt)
{
	if (stack.size() > 1) {
		auto it = stack.end() - 2;
		if (stack.back().get().render_previous()) {
			it->get().draw_scene(renderer, dt);
		}
	}
	stack.back().get().draw_scene(renderer, dt);
}

void StateManager::update(float dt)
{
	stack.back().get().update(dt);
}
