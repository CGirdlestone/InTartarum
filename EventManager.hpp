#pragma once

#include "World.hpp"
#include "BaseSystem.hpp"
#include "EventTypes.hpp"

class EventManager
{
private:
	std::map<EventTypes, std::vector<std::reference_wrapper<BaseSystem> > > subscribers;
	World& world;
public:
	EventManager(World& _world) :world(_world) {};
	~EventManager() {};

	void add_subscriber(EventTypes, BaseSystem&);
	void push_event(EventTypes event) const;
	void push_event(EventTypes event, uint32_t actor) const;
	void push_event(EventTypes event, uint32_t actor, uint32_t target) const;
	void push_event(EventTypes event, uint32_t actor, uint32_t target, uint32_t item) const;
};

