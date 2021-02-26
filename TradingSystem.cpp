#include "TradingSystem.hpp"

TradingSystem::TradingSystem(World& _world,  EventManager& _event_manager,  WorldMap& _world_map)
: world(_world), event_manager(_event_manager), world_map(_world_map)
{

}

TradingSystem::~TradingSystem()
{

}

void TradingSystem::update(float dt) 
{

}

void TradingSystem::on_tick() 
{

}

void TradingSystem::receive(EventTypes event) 
{

}

void TradingSystem::receive(EventTypes event, uint32_t actor) 
{

}

void TradingSystem::receive(EventTypes event, uint32_t actor, uint32_t target) 
{

}

void TradingSystem::receive(EventTypes event, uint32_t actor, uint32_t target, uint32_t item) 
{

}

