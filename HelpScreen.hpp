#pragma once

#include "Common.hpp"
#include "Keyboard.hpp"
#include "BaseState.hpp"
#include "StateManager.hpp"
#include "World.hpp"
#include "Utils.hpp"

class HelpScreen : public BaseState
{
public:
	HelpScreen(StateManager& _state_manager, World& _world, EventManager& _event_manager, Keyboard& _keyboard);
	~HelpScreen();
	
	virtual void handle_input(SDL_Event& event) override;
	virtual void on_tick() override;
	virtual void update(float dt) override;
	virtual void draw_scene(Renderer& renderer, const uint32_t fps, float dt) const override;
	virtual bool render_previous() const override;
	virtual GameState get_state() const override;
	virtual void on_entrance(Renderer& renderer) override;
	virtual void on_bury() const override;
	void load_controls();
private:
	StateManager& state_manager;
	World& world;
	EventManager& event_manager;
	Keyboard& keyboard;
	bool render_prev{ false };
	GameState state{ GameState::HELP };
	std::vector<std::string> controls;
};
