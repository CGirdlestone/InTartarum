#pragma once

#include "BaseSystem.hpp"
#include "EventManager.hpp"
#include "SoundManager.hpp"
#include "TextureManager.hpp"
#include "MessageLog.hpp"
#include "EntityFactory.hpp"
#include "SoundSystem.hpp"
#include "InventorySystem.hpp"
#include "CombatSystem.hpp"
#include "Prefabs.hpp"
#include "World.hpp"
#include "WorldMap.hpp"

using Script = void(*)(World& world, WorldMap& world_map, EventManager& event_manager, SoundManager& sound_manager, TextureManager& texture_manager, int tile_width, int tile_height, uint32_t entity, uint32_t target, uint32_t item);

int lua_print(lua_State* vm);

class ScriptSystem : public BaseSystem
{
private:
	World& world;
	WorldMap& world_map;
	EventManager& event_manager;
	MessageLog& message_log;
	EntityFactory& entity_factory;
	SoundSystem& sound_system;
	SoundManager& sound_manager;
	TextureManager& texture_manager;
	InventorySystem& inventory_system;
	CombatSystem& combat_system;
	int tile_width{ 0 };
	int tile_height{ 0 };
	std::unique_ptr<lua_State, decltype(&lua_close)> Lua_VM;

	void prepare_vm(uint32_t actor, uint32_t target, uint32_t item);

	std::map<std::string, Script> bump_scripts;
	void load_bump_scripts();
	void do_bump(uint32_t entity);

	std::map<std::string, Script> update_scripts;
	void load_update_scripts();
	void do_update(uint32_t entity);

	std::map<std::string, Script> death_scripts;
	void load_death_scripts();
	void do_death(uint32_t entity);

	std::map<std::string, Script> consume_scripts;
	void load_consume_scripts();
	void do_consume(uint32_t actor, uint32_t item);

	std::map<std::string, Script> use_scripts;
	void load_use_scripts();
	void do_use(uint32_t actor, uint32_t target, uint32_t item);

	std::map<std::string, Script> equip_scripts;
	void load_equip_scripts();
	void do_equip(uint32_t entity, uint32_t item);

	std::map<std::string, Script> unequip_scripts;
	void load_unequip_scripts();
	void do_unequip(uint32_t entity, uint32_t item);


	void do_hit(uint32_t entity, uint32_t target, uint32_t item);

public:
	ScriptSystem(World& _world, 
					EventManager& _event_manager, 
					WorldMap& _world_map, 
					MessageLog& _message_log, 
					EntityFactory& _entity_factory, 
					SoundSystem& _sound_system, 
					SoundManager& _sound_manager, 
					TextureManager& _texture_manager,
					InventorySystem& _inventory_system,
					CombatSystem& _combat_system);
	~ScriptSystem();
	void init();

	virtual void update(float dt) override;
	virtual void on_tick() override;
	virtual void receive(EventTypes event) override;
	virtual void receive(EventTypes event, uint32_t actor) override;
	virtual void receive(EventTypes event, uint32_t actor, uint32_t item) override;
	virtual void receive(EventTypes event, uint32_t actor, uint32_t target, uint32_t item) override;
};

