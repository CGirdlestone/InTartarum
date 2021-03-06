#pragma once

#include "Common.hpp"
#include "Keyboard.hpp"
#include "BaseState.hpp"
#include "StateManager.hpp"
#include "World.hpp"
#include "Utils.hpp"
#include "CharacterClass.hpp"
#include "EntityFactory.hpp"

class CharacterCreationScreen : public BaseState
{
private:
	StateManager& state_manager;
	World& world;
	TextureManager& tex_manager;
	EventManager& event_manager;
	Keyboard& keyboard;
	EntityFactory& entity_factory;
	int tile_width{ 0 };
	int tile_height{ 0 };
	unsigned int tileset{ 0 };
	bool render_prev{ false };
	std::string player_name{ "" };
	std::map<int, CharacterClass> char_options;
	std::vector<std::string> stats;
	int selection{ 0 };
	int world_x{ 0 };
	int world_y{ 0 };

	bool load_character_classes();
	void create_player();

public:
	CharacterCreationScreen(StateManager& _state_manager, World& _world, TextureManager& _tex_manager, EventManager& _event_manager, Keyboard& _keyboard, EntityFactory& _entity_factory, int _tile_width, int _tile_height, unsigned int _tileset, int _world_x, int _world_y);
	~CharacterCreationScreen() {};
	virtual void handle_input(SDL_Event& event) override;
	virtual void on_tick() override;
	virtual void update(float dt) override;
	virtual void draw_scene(Renderer& renderer, const uint32_t fps, float dt) const override;
	inline virtual bool render_previous() const override {	return render_prev;	};
	inline virtual GameState get_state() const override { return GameState::CHARACTER_CREATION; };
	virtual void on_entrance(Renderer& renderer) override;
	virtual void on_bury() const override;
};

