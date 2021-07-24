#include "PlayerSystem.hpp"

PlayerSystem::PlayerSystem(World& _world,  EventManager& _event_manager,  WorldMap& _world_map)
: world(_world), event_manager(_event_manager), world_map(_world_map)
{
	event_manager.add_subscriber(EventTypes::EXP_GAIN, *this);
}

PlayerSystem::~PlayerSystem()
{

}

void PlayerSystem::update(float dt) 
{

}

void PlayerSystem::on_tick() 
{

}

void PlayerSystem::receive(EventTypes event) 
{

}

void PlayerSystem::receive(EventTypes event, uint32_t actor) 
{
	switch (event) {
	case EventTypes::EXP_GAIN: add_xp(actor);
	}
}

void PlayerSystem::receive(EventTypes event, uint32_t actor, uint32_t target) 
{

}

void PlayerSystem::receive(EventTypes event, uint32_t actor, uint32_t target, uint32_t item) 
{

}

void PlayerSystem::add_xp(uint32_t exp)
{
	uint32_t player_entity = world.GetEntitiesWith<Player>()[0];
	auto* player = world.GetComponent<Player>(player_entity);

	player->xp += exp;
}

