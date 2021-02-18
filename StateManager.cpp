#include "StateManager.hpp"

StateManager::StateManager(World& _world, Renderer& _renderer, EventManager& _event_manager) 
	: world(_world), renderer(_renderer), event_manager(_event_manager)
{
	
}

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

void StateManager::draw_scene(uint32_t fps, float dt)
{
	renderer.Clear();

	if (stack.size() > 1) {
		auto it = stack.end() - 1;
		for (; it != stack.begin(); it--) {
			if (!it->get().render_previous()) {
				break;
			}
		}

		do {
			it->get().draw_scene(renderer, fps, dt);
		} while (++it != stack.end());
	}
	stack.back().get().draw_scene(renderer, fps, dt);
	renderer.Update();
}

void StateManager::update(float dt)
{
	stack.back().get().update(dt);
}
