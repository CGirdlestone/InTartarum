#pragma once

#include "Common.hpp"
#include "Keyboard.hpp"
#include "BaseState.hpp"
#include "StateManager.hpp"
#include "World.hpp"
#include "Utils.hpp"

class InventoryScreen :
    public BaseState
{
public:

	InventoryScreen(StateManager& _state_manager, World& _world, EventManager& _event_manager, Keyboard& _keyboard) :
		state_manager(_state_manager), world(_world), event_manager(_event_manager), keyboard(_keyboard) {};
	~InventoryScreen() {};

	virtual void handle_input(SDL_Event& event) override;
	virtual void on_tick() override;
	virtual void update(float dt) override;
	virtual void draw_scene(Renderer& renderer, const uint32_t fps, float dt) const override;
	virtual bool render_previous() const override;
	virtual GameState get_state() const override;
	virtual void on_entrance(Renderer& renderer) const override;
	virtual void on_bury() const override;

private:
	StateManager& state_manager;
	World& world;
	EventManager& event_manager;
	Keyboard& keyboard;
	bool render_prev{ false };
	GameState state{ GameState::INVENTORY };
	int option{ 0 };
};

