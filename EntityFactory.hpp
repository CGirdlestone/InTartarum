#pragma once
#include "World.hpp"
#include "TextureManager.hpp"
#include "Utils.hpp"

class EntityFactory
{
public:
	EntityFactory(World& _world, TextureManager& _texture_manager);
	~EntityFactory() {};

	uint32_t create_mob(std::string& entity_name, int x, int y, int z, int world_x, int world_y);
	uint32_t create_player(int world_x, int world_y);
	uint32_t create_prop(std::string& entity_name, int x, int y, int z, int world_x, int world_y);
	uint32_t create_item(std::string& entity_name, int x, int y, int z, int world_x, int world_y);
	uint32_t create_item(std::string& entity_name); // used to create items that are owned (inventory or equipped) by an actor
	uint32_t create_npc(std::string& entity_name, int x, int y, int z, int world_x, int world_y);

private:
	World& world;
	TextureManager& texture_manager;
	
	bool load_file(SmartLuaVM& vm, const std::string& lua_file);
	void create_entity(SmartLuaVM& vm, uint32_t& entity);
	void create_position(SmartLuaVM& vm, uint32_t& entity);
	void create_sprite(SmartLuaVM& vm, uint32_t& entity);
	void create_player(SmartLuaVM& vm, uint32_t& entity);
	void create_actor(SmartLuaVM& vm, uint32_t& entity);
	void create_blocker(SmartLuaVM& vm, uint32_t& entity);
	void create_interactable(SmartLuaVM& vm, uint32_t& entity);
	void create_animation(SmartLuaVM& vm, uint32_t& entity);
	void create_particle(SmartLuaVM& vm, uint32_t& entity);
	void create_script(SmartLuaVM& vm, uint32_t& entity);
	void create_item(SmartLuaVM& vm, uint32_t& entity);
	void create_equipable(SmartLuaVM& vm, uint32_t& entity);
	void create_weapon(SmartLuaVM& vm, uint32_t& entity);
	void create_container(SmartLuaVM& vm, uint32_t& entity);
	void create_stackable(SmartLuaVM& vm, uint32_t& entity);
	void create_body(SmartLuaVM& vm, uint32_t& entity);
	void create_id(SmartLuaVM& vm, uint32_t& entity);
	void create_ai(SmartLuaVM& vm, uint32_t& entity);
	void create_useable(SmartLuaVM& vm, uint32_t& entity);

	std::vector<uint32_t> load_starting_equipment();
};


