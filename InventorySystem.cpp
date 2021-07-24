#include "InventorySystem.hpp"

InventorySystem::InventorySystem(World& _world, EventManager& _event_manager, WorldMap& _world_map, EntityFactory& _entity_factory)
	: world(_world), event_manager(_event_manager), world_map(_world_map), entity_factory(_entity_factory)
{
	event_manager.add_subscriber(EventTypes::TRY_PICK_UP_ITEM, *this);
	event_manager.add_subscriber(EventTypes::DROP_ITEM, *this);
	event_manager.add_subscriber(EventTypes::DROP_ITEM_STACK, *this);
	event_manager.add_subscriber(EventTypes::EQUIP_ITEM, *this);
	event_manager.add_subscriber(EventTypes::UNEQUIP_ITEM, *this);
	event_manager.add_subscriber(EventTypes::DROP_EQUIPPED_ITEM, *this);
	event_manager.add_subscriber(EventTypes::USE_ITEM, *this);
	event_manager.add_subscriber(EventTypes::CONSUME, *this);
	event_manager.add_subscriber(EventTypes::DECREASE_CHARGE, *this);
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

void InventorySystem::receive(EventTypes event, uint32_t entity)
{
	switch (event) {
	case EventTypes::TRY_PICK_UP_ITEM: pick_up(entity); break;
	}
}

void InventorySystem::receive(EventTypes event, uint32_t actor, uint32_t item)
{
	switch (event) {
	case EventTypes::DROP_ITEM: drop(actor, item); break;
	case EventTypes::EQUIP_ITEM: equip(actor, item); break;
	case EventTypes::UNEQUIP_ITEM: unequip(actor, item); break;
	case EventTypes::DROP_EQUIPPED_ITEM: drop_equipped(actor, item); break;
	case EventTypes::CONSUME: use_consumable(actor, item); break;
	case EventTypes::DECREASE_CHARGE: decrease_charges(actor, item); break;
	}
}

void InventorySystem::receive(EventTypes event, uint32_t actor, uint32_t item, uint32_t quantity)
{
	switch (event) {
	case EventTypes::DROP_ITEM_STACK: drop_stack(actor, item, quantity); break;
	case EventTypes::USE_ITEM: use_targetable(actor, item, quantity); break;
	}
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
	world.AddComponent<Position>(item, pos->x, pos->y, pos->z, pos->world_x, pos->world_y);
	world_map.get_entity_grid().add_entity(item, pos->x, pos->y);
	event_manager.push_event(EventTypes::DROP_ITEM_MESSAGE, item);
}

void InventorySystem::drop_stack(uint32_t actor, uint32_t item, uint32_t quantity)
{
	auto* stack = world.GetComponent<Stackable>(item);
	auto* _item = world.GetComponent<Item>(item);
	auto* pos = world.GetComponent<Position>(actor);
	auto* container = world.GetComponent<Container>(actor);

	if (static_cast<int>(quantity) <= stack->quantity) {
		auto* id = world.GetComponent<ID>(item);
		std::string entity_name = id->id;
		auto entity = entity_factory.create_item(entity_name, pos->x, pos->y, pos->z, pos->world_x, pos->world_y);
		auto* new_stack = world.GetComponent<Stackable>(entity);
		new_stack->quantity = quantity;
		if (static_cast<int>(quantity) == stack->quantity) {
			remove_from_inventory(actor, item);
		}
		else {
			stack->quantity -= quantity;
			container->weight -= _item->weight * quantity;
		}
		world_map.get_entity_grid().add_entity(entity, pos->x, pos->y);
		event_manager.push_event(EventTypes::DROP_ITEM_STACK_MESSAGE, entity, quantity);
	}
}

void InventorySystem::equip(uint32_t actor, uint32_t item)
{
	auto* equipable = world.GetComponent<Equipable>(item);
	
	if (equipable != nullptr) {
		auto slot = equipable->slot;
		auto* body = world.GetComponent<Body>(actor);
		if (is_slot_occupied(actor, item)) {
			auto equipped_item = body->equipment[static_cast<int>(slot)];
			add_to_inventory(actor, equipped_item);
			body->equipment[static_cast<int>(slot)] = item;
		}

		remove_from_inventory(actor, item);
		body->equipment[static_cast<int>(slot)] = item;
		auto* script = world.GetComponent<Scriptable>(item);
		if (script != nullptr) {
			if (script->OnEquip != "") {
				event_manager.push_event(EventTypes::ON_EQUIP_SCRIPT, actor, item);
			}
		}
	}
}

void InventorySystem::unequip(uint32_t actor, uint32_t item)
{
	auto* equipable = world.GetComponent<Equipable>(item);
	auto* body = world.GetComponent<Body>(actor);
	// if actor has space in their inventory, add the item to it.
	if (can_pick_up(actor, item)) {
		add_to_inventory(actor, item);
	}

	body->equipment[static_cast<int>(equipable->slot)] = MAX_ENTITIES + 1;
	auto* script = world.GetComponent<Scriptable>(item);
	if (script != nullptr) {
		if (script->OnUnequip != "") {
			event_manager.push_event(EventTypes::ON_UNEQUIP_SCRIPT, actor, item);
		}
	}
}

void InventorySystem::drop_equipped(uint32_t actor, uint32_t item)
{
	auto* stack = world.GetComponent<Stackable>(item);
	event_manager.push_event(EventTypes::UNEQUIP_ITEM, actor, item);
	if (stack == nullptr) {
		event_manager.push_event(EventTypes::DROP_ITEM, actor, item);
	}
	else {
		event_manager.push_event(EventTypes::DROP_ITEM_STACK, actor, item, static_cast<uint32_t>(stack->quantity));
	}
}

void InventorySystem::use_consumable(uint32_t actor, uint32_t item)
{
	auto* script = world.GetComponent<Scriptable>(item);
	event_manager.push_event(EventTypes::ON_USE_SCRIPT, actor, item);
}

void InventorySystem::use_targetable(uint32_t actor, uint32_t target, uint32_t item)
{
	auto* script = world.GetComponent<Scriptable>(item);
	event_manager.push_event(EventTypes::ON_USE_SCRIPT, actor, target, item);
}

void InventorySystem::decrease_charges(uint32_t owner, uint32_t item)
{
	auto* useable = world.GetComponent<Useable>(item);
	// -1 means unlimited uses
	if (useable->charges == -1) {
		return;
	}

	useable->charges -= 1;
	if (useable->charges == 0) {
		auto* stack = world.GetComponent<Stackable>(item);
		if (stack != nullptr) {
			stack->quantity -= 1;
			if (stack->quantity == 0) {
				remove_from_inventory(owner, item);
				world.KillEntity(item);
			}
		}
		else {
			remove_from_inventory(owner, item);
			world.KillEntity(item);
		}
	}
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
