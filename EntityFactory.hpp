#pragma once
#include "World.hpp"
#include "TextureManager.hpp"
#include "Utils.hpp"

class EntityFactory
{
public:
	EntityFactory(World& _world, TextureManager& _texture_manager);
	~EntityFactory() {};

	uint32_t create_mob(std::string& entity_name, int x, int y, int z);
	uint32_t create_player();
	uint32_t create_item(std::string& entity_name, int x, int y, int z);
	uint32_t create_item(std::string& entity_name); // used to create items that are owned (inventory or equipped) by an actor
	uint32_t create_npc(std::string& entity_name, int x, int y, int z);
private:
	World& world;
	TextureManager& texture_manager;
	SmartLuaVM vm;
	
	bool load_file(const std::string& lua_file);
	void create_entity(uint32_t& entity);
	void create_position(uint32_t& entity);
	void create_sprite(uint32_t& entity);
	void create_player(uint32_t& entity);
	void create_actor(uint32_t& entity);
	void create_blocker(uint32_t& entity);
	void create_interactable(uint32_t& entity);
	void create_animation(uint32_t& entity);
	void create_particle(uint32_t& entity);
	void create_script(uint32_t& entity);
	void create_item(uint32_t& entity);
	void create_equipable(uint32_t& entity);
	void create_weapon(uint32_t& entity);
	void create_container(uint32_t& entity);
};


