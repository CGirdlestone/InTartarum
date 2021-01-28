#pragma once

#include "BaseSystem.hpp"
#include "EventManager.hpp"
#include "World.hpp"
#include "WorldMap.hpp"

using Script = void(*)(World& world, uint32_t entity);

class ScriptSystem : public BaseSystem
{
private:
	World& world;
	EventManager& event_manager;
	WorldMap& world_map;

	std::map<std::string, Script> bump_scripts;
	void load_bump_scripts();
	void do_bump(uint32_t entity);

	std::map<std::string, Script> update_scripts;
	void load_update_scripts();
	void do_update(uint32_t entity);
public:
	ScriptSystem(World& _world, EventManager& _event_manager, WorldMap& _world_map);
	~ScriptSystem() { };
	void init();
	virtual void update(float dt) override;
	virtual void on_tick() override;
	virtual void receive(EventTypes event) override;
	virtual void receive(EventTypes event, uint32_t actor) override;
	virtual void receive(EventTypes event, uint32_t actor, uint32_t target) override;
	virtual void receive(EventTypes event, uint32_t actor, uint32_t target, uint32_t item) override;
};

