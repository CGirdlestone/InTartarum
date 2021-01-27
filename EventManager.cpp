#include "EventManager.hpp"

void EventManager::add_subscriber(EventTypes event, BaseSystem& s)
{
	if (subscribers.find(event) == subscribers.end()) {
		subscribers.insert({ event, {std::reference_wrapper<BaseSystem>(s)} });
	}
	else {
		subscribers.at(event).push_back(std::reference_wrapper<BaseSystem>(s));
	}
}

void EventManager::push_event(EventTypes event) const
{
	for (const auto& s : subscribers.at(event)) {
		s.get().receive(event);
	}
}

void EventManager::push_event(EventTypes event, uint32_t actor) const
{
	for (const auto& s : subscribers.at(event)) {
		s.get().receive(event, actor);
	}
}

void EventManager::push_event(EventTypes event, uint32_t actor, uint32_t target) const
{
	for (const auto& s : subscribers.at(event)) {
		s.get().receive(event, actor, target);
	}
}

void EventManager::push_event(EventTypes event, uint32_t actor, uint32_t target, uint32_t item) const
{
	for (const auto& s : subscribers.at(event)) {
		s.get().receive(event, actor, target, item);
	}
}
