#include "MessageLog.hpp"

MessageLog::MessageLog(World& _world, EventManager& _event_manager, int _width, int _height)
	: world(_world), event_manager(_event_manager), randomiser(), width(_width), height(_height) 
{ 
	event_manager.add_subscriber(EventTypes::MELEE_HIT, *this);
	event_manager.add_subscriber(EventTypes::RANGED_HIT, *this);
	event_manager.add_subscriber(EventTypes::SPELL_HIT, *this);
	event_manager.add_subscriber(EventTypes::CRITICAL_MELEE_HIT, *this);
	event_manager.add_subscriber(EventTypes::CRITICAL_RANGED_HIT, *this);
	event_manager.add_subscriber(EventTypes::CRITICAL_SPELL_HIT, *this);
	event_manager.add_subscriber(EventTypes::DEATH, *this);
	event_manager.add_subscriber(EventTypes::LEVEL_UP, *this);
	event_manager.add_subscriber(EventTypes::MESSAGE, *this);
	event_manager.add_subscriber(EventTypes::BLOCKED_MOVEMENT, *this);
}

void MessageLog::update(float dt)
{

}

void MessageLog::on_tick()
{

}

void MessageLog::receive(EventTypes event)
{
	switch (event) {
	case EventTypes::MELEE_HIT: {
		auto msg = Message("Testing... testing... 1 2 3...");
		add_message(msg);
		break;
	}
	case EventTypes::BLOCKED_MOVEMENT: {
		auto msg = Message("You can't move there!");
		add_message(msg);
		break;
	}
	}
}

void MessageLog::receive(EventTypes event, uint32_t actor)
{

}

void MessageLog::receive(EventTypes event, uint32_t actor, uint32_t target)
{

}

void MessageLog::receive(EventTypes event, uint32_t actor, uint32_t target, uint32_t item)
{

}

void MessageLog::serialise(std::ofstream& file)
{

}

void MessageLog::deserialise(const char* buffer, size_t& offset)
{

}

void MessageLog::load_descriptions(const char* path)
{

}

void MessageLog::add_message(Message& message)
{
	message_queue.push_back(message);
	if (message_queue.size() == message_history) {
		message_queue.pop_front();
	}
}


