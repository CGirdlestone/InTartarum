#pragma once

#include "Common.hpp"
#include "EventTypes.hpp"

class BaseSystem
{
public:
	virtual ~BaseSystem() = 0 { };
	virtual void update(float dt) = 0;
	virtual void on_tick() = 0;
	virtual void receive(EventTypes event) = 0;
	virtual void receive(EventTypes event, uint32_t actor) = 0;
	virtual void receive(EventTypes event, uint32_t actor, uint32_t target) = 0;
	virtual void receive(EventTypes event, uint32_t actor, uint32_t target, uint32_t item) = 0;
};
