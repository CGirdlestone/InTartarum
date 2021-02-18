#include "InventorySystem.hpp"

InventorySystem::InventorySystem(World& _world, EventManager& _event_manager, WorldMap& _world_map) 
	: world(_world), event_manager(_event_manager), world_map(_world_map) 
{
	event_manager.add_subscriber(EventTypes::TRY_PICK_UP_ITEM, *this);
	event_manager.add_subscriber(EventTypes::DROP_ITEM, *this);
	event_manager.add_subscriber(EventTypes::DROP_ITEM_STACK, *this);
	event_manager.add_subscriber(EventTypes::EQUIP_ITEM, *this);
	event_manager.add_subscriber(EventTypes::UNEQUIP_ITEM, *this);
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
	switch (event) {
	case EventTypes::DROP_ITEM: drop(actor, target);
	}
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

	if (entities.size() == 1) {
		// it will always be at least 1 as the player will be in the list.
		event_manager.push_event(EventTypes::NO_ITEM_PRESENT);
		return;
	}

	// we can only pick up one item at a time but we still look to allow us to skip the player or any mobs and still access items.
	for (auto e : entities) {
		auto* item = world.GetComponent<Item>(e);
		if (item == nullptr) {
			continue;
		}
		// check whether the actor has enough free weight to pick the item up
		if (!can_pick_up(actor, e)) {
			return;
		}
		// handle stackable items
		if (world.GetComponent<Stackable>(e) != nullptr) {
			auto current = get_item_from_inventory(actor, item->name);
			// actor has a current stack of this item in the inventory
			if (current != MAX_ENTITIES + 1) {
				add_to_stack(actor, e, current);
				world.KillEntity(e);

				// handle removing item here as a special case as world.KillEntity will make the the message fail as it tries to access a deleted component
				// on the entity 'e', so instead we use the data from the stack in the player's inventory.
				world_map.get_entity_grid().remove_entity(e, pos->x, pos->y);
				if (player != nullptr) {
					event_manager.push_event(EventTypes::PICK_UP_ITEM, current);
				}
				break;
			}
			// actor does not have a current stack of this item in the inventory
			else {
				add_to_inventory(actor, e);
				world.RemoveComponent<Position>(e);
			}
		}
		// handle items that are not stackable
		else {
			add_to_inventory(actor, e);
			world.RemoveComponent<Position>(e);	
		}
		//remove the item from the map and add a message to the log if the actor is the player.
		world_map.get_entity_grid().remove_entity(e, pos->x, pos->y);
		if (player != nullptr) {
			event_manager.push_event(EventTypes::PICK_UP_ITEM, e);
		}
		
		break;
	}
}

void InventorySystem::drop(uint32_t actor, uint32_t item)
{
	remove_from_inventory(actor, item);
	auto* pos = world.GetComponent<Position>(actor);
	world.AddComponent<Position>(item, pos->x, pos->y, pos->z);
	world_map.get_entity_grid().add_entity(item, pos->x, pos->y);
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
	auto* stack = world.GetComponent<Stackable>(item);
	int quantity = stack == nullptr ? 1 : stack->quantity;

	if (container->weight + _item->weight * quantity <= container->weight_capacity) {
		return true;
	}

	event_manager.push_event(EventTypes::OVERWEIGHT);
	return false;
}

bool InventorySystem::is_slot_occupied(uint32_t actor, uint32_t item)
{
	// TO DO
	return false;
}

void InventorySystem::add_to_inventory(uint32_t actor, uint32_t item)
{
	auto* container = world.GetComponent<Container>(actor);
	auto* _item = world.GetComponent<Item>(item);
	
	container->inventory.push_back(item);
	container->weight += _item->weight;
}

void InventorySystem::add_to_stack(uint32_t actor, uint32_t item, uint32_t current)
{
	auto* container = world.GetComponent<Container>(actor);
	auto* _item = world.GetComponent<Item>(item);
	auto* new_stack = world.GetComponent<Stackable>(item);
	auto* current_stack = world.GetComponent<Stackable>(current);

	current_stack->quantity += new_stack->quantity;
	container->weight += _item->weight * new_stack->quantity;
}

uint32_t InventorySystem::get_item_from_inventory(uint32_t actor, const std::string& item)
{
	auto* container = world.GetComponent<Container>(actor);

	auto item_by_name = [this, item](const uint32_t& e) {
		auto* item_component = this->world.GetComponent<Item>(e);
		return item_component->name == item;
	};
	auto it = std::find_if(container->inventory.begin(), container->inventory.end(), item_by_name);

	if (it != container->inventory.end()) {
		return *it;
	}
	else {
		return MAX_ENTITIES + 1;
	}
}

void InventorySystem::remove_from_inventory(uint32_t actor, uint32_t item)
{
	auto* container = world.GetComponent<Container>(actor);

	container->inventory.erase(std::remove_if(container->inventory.begin(), container->inventory.end(), [item](const uint32_t e) { return item == e; }), container->inventory.end());
}
