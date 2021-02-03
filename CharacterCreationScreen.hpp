#pragma once

#include "Common.hpp"
#include "BaseState.hpp"
#include "StateManager.hpp"
#include "World.hpp"
#include "Utils.hpp"
#include "CharacterClass.hpp"

class CharacterCreationScreen : public BaseState
{
private:
	StateManager& state_manager;
	World& world;
	TextureManager& tex_manager;
	EventManager& event_manager;
	bool render_prev{ false };
	std::string player_name{ "" };
	std::map<int, CharacterClass> char_options;
	int selection{ 0 };

public:
	CharacterCreationScreen(StateManager& _state_manager, World& _world, TextureManager& _tex_manager, EventManager& _event_manager);
	~CharacterCreationScreen() {};
	virtual void handle_input(SDL_Event& event) override;
	virtual void on_tick() override;
	virtual void update(float dt) override;
	virtual void draw_scene(Renderer& renderer, const uint32_t fps, float dt) const override;
	inline virtual bool render_previous() const override {	return render_prev;	};
	inline virtual GameState get_state() const override { return GameState::CHARACTER_CREATION; };
	virtual void on_entrance(Renderer& renderer) const override;
	virtual void on_bury() const override;
};

