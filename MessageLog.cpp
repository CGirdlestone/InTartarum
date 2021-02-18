#include "MessageLog.hpp"

MessageLog::MessageLog(World& _world, EventManager& _event_manager)
	: world(_world), event_manager(_event_manager), randomiser(), width(0), height(0)
{
	SmartLuaVM vm(nullptr, &lua_close);
	vm.reset(luaL_newstate());
	auto result = luaL_dofile(vm.get(), "./Config/window.lua");

	if (result == LUA_OK) {
		lua_getglobal(vm.get(), "message_log_width");
		if (lua_isnumber(vm.get(), -1)) {
			width = static_cast<int>(lua_tonumber(vm.get(), -1));
		}
		lua_getglobal(vm.get(), "message_log_height");
		if (lua_isnumber(vm.get(), -1)) {
			height = static_cast<int>(lua_tonumber(vm.get(), -1));
		}
		lua_getglobal(vm.get(), "message_log_number_msgs");
		if (lua_isnumber(vm.get(), -1)) {
			message_history = static_cast<uint32_t>(lua_tonumber(vm.get(), -1));
		}
		lua_getglobal(vm.get(), "message_log_num_lines");
		if (lua_isnumber(vm.get(), -1)) {
			num_lines = static_cast<uint32_t>(lua_tonumber(vm.get(), -1));
		}
	}

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
	event_manager.add_subscriber(EventTypes::PICK_UP_ITEM, *this);
	event_manager.add_subscriber(EventTypes::NO_ITEM_PRESENT, *this);
	event_manager.add_subscriber(EventTypes::DROP_ITEM_MESSAGE, *this);
	event_manager.add_subscriber(EventTypes::DROP_ITEM_STACK_MESSAGE, *this);
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
	case EventTypes::NO_ITEM_PRESENT: {
		auto msg = Message("There's nothing to pick up here!");
		add_message(msg);
		break;
	}
	}
}

void MessageLog::receive(EventTypes event, uint32_t actor)
{
	switch (event) {
	case EventTypes::PICK_UP_ITEM: {
		auto* item = world.GetComponent<Item>(actor);
		std::string text = "You pick up the $.";
		interpolate(text, item->name);
		auto msg = Message(text);
		add_message(msg);
		break;
	}
	case EventTypes::DROP_ITEM_MESSAGE: {
		auto* item = world.GetComponent<Item>(actor);
		std::string text = "You drop up the $ on the ground.";
		interpolate(text, item->name);
		auto msg = Message(text);
		add_message(msg);
		break;
	}
	}
}

void MessageLog::receive(EventTypes event, uint32_t actor, uint32_t target)
{
	switch (event) {
	case EventTypes::DROP_ITEM_STACK_MESSAGE: {
		auto* item = world.GetComponent<Item>(actor);
		auto quantity = static_cast<int>(target);
		std::string text{ "" };
		if (quantity == 1) {
			text = "You drop the $ on the ground.";
			interpolate(text, item->name);
		}
		else {
			text = "You drop $ $s on the ground.";
			interpolate(text, quantity, item->name);
		}
		auto msg = Message(text);
		add_message(msg);
		break;
	}
	}
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


