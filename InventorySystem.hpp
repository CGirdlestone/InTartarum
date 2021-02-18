#pragma once

#include "EventManager.hpp"
#include "EntityFactory.hpp"
#include "WorldMap.hpp"

class InventorySystem : public BaseSystem
{
public:
	InventorySystem(World& _world, EventManager& _event_manager, WorldMap& _world_map, EntityFactory& _entity_factory);
	~InventorySystem() {};

	virtual void update(float dt) override;
	virtual void on_tick() override;
	virtual void receive(EventTypes event) override;
	virtual void receive(EventTypes event, uint32_t actor) override;
	virtual void receive(EventTypes event, uint32_t actor, uint32_t target) override;
	virtual void receive(EventTypes event, uint32_t actor, uint32_t target, uint32_t item) override;

private:
	World& world;
	EventManager& event_manager;
	WorldMap& world_map;
	EntityFactory& entity_factory;

	void pick_up(uint32_t actor);
	void drop(uint32_t actor, uint32_t item);
	void drop_stack(uint32_t actor, uint32_t item, uint32_t quantity);
	void equip(uint32_t actor, uint32_t item);
	void unequip(uint32_t actor, uint32_t item);

	bool can_pick_up(uint32_t actor, uint32_t item);
	bool is_slot_occupied(uint32_t actor, uint32_t item);
	void add_to_inventory(uint32_t actor, uint32_t item);
	void add_to_stack(uint32_t actor, uint32_t item, uint32_t current);
	uint32_t get_item_from_inventory(uint32_t actor, const std::string& item);
	void remove_from_inventory(uint32_t actor, uint32_t item);
	inline std::vector<uint32_t>& get_entities_at_tile(int x, int y) { return world_map.get_entity_grid().get(x, y); };
};

