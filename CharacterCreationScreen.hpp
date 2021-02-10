#pragma once

#include "Common.hpp"
#include "Keyboard.hpp"
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
	Keyboard& keyboard;
	int tile_width{ 0 };
	int tile_height{ 0 };
	unsigned int tileset{ 0 };
	bool render_prev{ true };
	std::string player_name{ "" };
	std::map<int, CharacterClass> char_options;
	std::vector<std::string> stats;
	int selection{ 0 };

	bool load_character_classes();

public:
	CharacterCreationScreen(StateManager& _state_manager, World& _world, TextureManager& _tex_manager, EventManager& _event_manager, Keyboard& _keyboard, int _tile_width, int _tile_height, unsigned int _tileset);
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

