#include "InventorySystem.hpp"

InventorySystem::InventorySystem(World& _world, EventManager& _event_manager, WorldMap& _world_map) 
	: world(_world), event_manager(_event_manager), world_map(_world_map) 
{
	event_manager.add_subscriber(EventTypes::TRY_PICK_UP_ITEM, *this);
}

void InventorySystem::update(float dt)
{

}

void InventorySystem::on_tick()
{

}

void InventorySystem::receive(EventTypes event)
{

}

void InventorySystem::receive(EventTypes event, uint32_t actor)
{
	switch (event) {
	case EventTypes::TRY_PICK_UP_ITEM: pick_up(actor);
	}
}

void InventorySystem::receive(EventTypes event, uint32_t actor, uint32_t target)
{

}

void InventorySystem::receive(EventTypes event, uint32_t actor, uint32_t target, uint32_t item)
{

}

void InventorySystem::pick_up(uint32_t actor)
{
	auto* pos = world.GetComponent<Position>(actor);
	auto* container = world.GetComponent<Container>(actor);
	auto* player = world.GetComponent<Player>(actor);

	auto entities = get_entities_at_tile(pos->x, pos->y);

	if (entities.empty()) {
		event_manager.push_event(EventTypes::NO_ITEM_PRESENT);
		return;
	}
	for (auto e : entities) {
		auto* item = world.GetComponent<Item>(e);
		if (item == nullptr) {
			continue;
		}
		if (!can_pick_up(actor, e)) {
			event_manager.push_event(EventTypes::OVERWEIGHT);
			return;
		}
		container->inventory.push_back(e);
		world.RemoveComponent<Position>(e);
		world_map.get_entity_grid().remove_entity(e, pos->x, pos->y);
		if (player != nullptr) {
			event_manager.push_event(EventTypes::PICK_UP_ITEM, e);
		}
	}
}

void InventorySystem::drop(uint32_t actor, uint32_t item)
{

}

void InventorySystem::equip(uint32_t actor, uint32_t item)
{

}

void InventorySystem::unequip(uint32_t actor, uint32_t item)
{

}

bool InventorySystem::can_pick_up(uint32_t actor, uint32_t item)
{
	auto* container = world.GetComponent<Container>(actor);
	auto* _item = world.GetComponent<Item>(item);

	return container->weight + _item->weight <= container->weight_capacity;
}
