#pragma once

#include "Prefabs.hpp"
#include "StateManager.hpp"
#include "EventManager.hpp"
#include "WorldMap.hpp"
#include "Camera.hpp"
#include "Pathfinding.hpp"
#include "Keyboard.hpp"

class GameScreen : public BaseState, BaseSystem
{
private:
	StateManager& state_manager;
	World& world;
	TextureManager& tex_manager;
	EventManager& event_manager;
	WorldMap& world_map;
	Renderer& renderer;
	Camera& camera;
	MessageLog& message_log;
	Keyboard& keyboard;
	bool render_prev{ false };
	unsigned int id;
	unsigned int num_turns{ 0 };
	float lifetime{ 0.15f };
	float current{ 0.0f };

	void descend_dungeon();
	void return_to_safe_zone();
	void overworld_movement();
	void open_doors();
public:
	GameScreen(	StateManager& _state_manager, World& _world, TextureManager& _tex_manager, 
				EventManager& _event_manager, WorldMap& _world_map, Renderer& _renderer, 
				Camera& _camera, MessageLog& _message_log, Keyboard& _keyboard, 
				bool _render_prev, unsigned int _id);
	
	virtual void handle_input(SDL_Event& event) override;
	virtual void on_tick() override;
	virtual void update(float dt) override;
	virtual void draw_scene(Renderer& renderer, const uint32_t fps, float dt) const override;
	inline virtual bool render_previous() const override { return render_prev; };
	inline virtual GameState get_state() const override { return GameState::GAME; };
	virtual void on_entrance(Renderer& renderer) override;
	virtual void on_bury() const override;
	virtual void receive(EventTypes event) override;
	virtual void receive(EventTypes event, uint32_t actor)  override;
	virtual void receive(EventTypes event, uint32_t actor, uint32_t target)  override;
	virtual void receive(EventTypes event, uint32_t actor, uint32_t target, uint32_t item)  override;
	void load_game();
	void save_game();
};



