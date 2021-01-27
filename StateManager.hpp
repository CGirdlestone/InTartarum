#pragma once

#include "BaseState.hpp"
#include "World.hpp"
#include "Renderer.hpp"
#include "EventManager.hpp"

class StateManager {
private:
	std::map<GameState, std::reference_wrapper<BaseState> > states;
	std::vector<std::reference_wrapper<BaseState> > stack;
	World& world;
	Renderer& renderer;
	EventManager& event_manager;
	bool playing{ true };
public:
	StateManager(World& _world, Renderer& _renderer, EventManager& _event_manager) : world(_world), renderer(_renderer), event_manager(_event_manager) {};
	~StateManager() {};

	void add_state(GameState state, BaseState& state_instance);
	void push(GameState state);
	void pop(unsigned int n);
	void handle_input(SDL_Event& event);
	void draw_scene(uint32_t dt);
	void update(float dt);
	inline std::reference_wrapper<BaseState> get_state(GameState state) const { return states.at(state); };
	inline const bool is_playing() const { return playing; };
	void stop_playing() { playing = false; };
	inline Renderer& get_renderer() const { return renderer; };
};