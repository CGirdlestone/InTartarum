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
			num_lines = static_cast<int>(lua_tonumber(vm.get(), -1));
		}
	}

	event_manager.add_subscriber(EventTypes::MELEE_HIT, *this);
	event_manager.add_subscriber(EventTypes::RANGED_HIT, *this);
	event_manager.add_subscriber(EventTypes::SPELL_HIT, *this);
	event_manager.add_subscriber(EventTypes::CRITICAL_MELEE_HIT, *this);
	event_manager.add_subscriber(EventTypes::CRITICAL_RANGED_HIT, *this);
	event_manager.add_subscriber(EventTypes::CRITICAL_SPELL_HIT, *this);
	event_manager.add_subscriber(EventTypes::PLAYER_DEATH, *this);
	event_manager.add_subscriber(EventTypes::MOB_DEATH, *this);
	event_manager.add_subscriber(EventTypes::LEVEL_UP, *this);
	event_manager.add_subscriber(EventTypes::MESSAGE, *this);
	event_manager.add_subscriber(EventTypes::BLOCKED_MOVEMENT, *this);
	event_manager.add_subscriber(EventTypes::PICK_UP_ITEM, *this);
	event_manager.add_subscriber(EventTypes::NO_ITEM_PRESENT, *this);
	event_manager.add_subscriber(EventTypes::DROP_ITEM_MESSAGE, *this);
	event_manager.add_subscriber(EventTypes::DROP_ITEM_STACK_MESSAGE, *this);
	event_manager.add_subscriber(EventTypes::NO_USE, *this);
	event_manager.add_subscriber(EventTypes::INVALID_TARGET, *this);
	event_manager.add_subscriber(EventTypes::HEAL, *this);
	event_manager.add_subscriber(EventTypes::CAST, *this);
	event_manager.add_subscriber(EventTypes::AOE_CAST, *this);
	event_manager.add_subscriber(EventTypes::BUFF_HEALTH, *this);
	event_manager.add_subscriber(EventTypes::DEBUFF_HEALTH, *this);
	event_manager.add_subscriber(EventTypes::OUT_OF_RANGE, *this);
	event_manager.add_subscriber(EventTypes::DEAL_DAMAGE, *this);
	event_manager.add_subscriber(EventTypes::EXP_GAIN, *this);
	
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
	case EventTypes::NO_USE: {
		auto msg = Message("You can't use that!");
		add_message(msg);
		break;
	}
	case EventTypes::INVALID_TARGET: {
		auto msg = Message("You must select a valid target!");
		add_message(msg);
		break;
	}
	case EventTypes::OUT_OF_RANGE: {
		auto msg = Message("Target out of range!");
		add_message(msg);
		break;
	}
	case EventTypes::PLAYER_DEATH: {
		auto msg = Message("You have been slain!");
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
	case EventTypes::MOB_DEATH: {
		auto* mob = world.GetComponent<Actor>(actor);
		std::string text = "The $ has been slain.";
		interpolate(text, mob->name);
		auto msg = Message(text);
		add_message(msg);
		break;
	}
	case EventTypes::EXP_GAIN: {
		std::string text = "You gain $ xp.";
		interpolate(text, actor);
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
	case EventTypes::BUFF_HEALTH: {
		auto* player = world.GetComponent<Player>(actor);
		if (player == nullptr) {
			return;
		}
		auto* item = world.GetComponent<Item>(target);
		std::string text{ "You equip the $ and feel stronger!" };
		interpolate(text, item->name);
		auto msg = Message(text);
		add_message(msg);
		break;
	}
	case EventTypes::DEBUFF_HEALTH: {
		auto* player = world.GetComponent<Player>(actor);
		if (player == nullptr) {
			return;
		}
		auto* item = world.GetComponent<Item>(target);
		std::string text{ "You unequip the $ and feel weaker!" };
		interpolate(text, item->name);
		auto msg = Message(text);
		add_message(msg);
		break;
	}
	case EventTypes::AOE_CAST: {
		auto* item = world.GetComponent<Item>(target);
		std::string text{ "You cast $!" };
		interpolate(text, item->name);
		auto msg = Message(text);
		add_message(msg);
		break;
	}
	case EventTypes::HEAL: {
		auto* entity = world.GetComponent<Actor>(actor);
		if (entity != nullptr) {
			auto* player = world.GetComponent<Player>(actor);
			if (player == nullptr) {
				std::string text = "You quaff the potion and heal for $!";
				interpolate(text, target);
				auto msg = Message(text);
				add_message(msg);
			}
			else {
				std::string text = "$ quaffs the potion and heals for $!";
				interpolate(text, entity->name, target);
				auto msg = Message(text);
				add_message(msg);
			}
		}
		break;
	}
	}
}

void MessageLog::receive(EventTypes event, uint32_t actor, uint32_t target, uint32_t item)
{
	switch (event) {
	case EventTypes::CAST: {
		auto* user = world.GetComponent<Actor>(actor);
		auto* enemy = world.GetComponent<Actor>(target);
		auto* used_item = world.GetComponent<Scriptable>(item);

		std::string text{ "$ casts $ at $." };
		interpolate(text, user->name, used_item->OnUse, enemy->name);
		auto msg = Message(text);
		add_message(msg);
		break;
	}
	case EventTypes::DEAL_DAMAGE: {
		auto* user = world.GetComponent<Actor>(actor);
		auto* enemy = world.GetComponent<Actor>(target);
		auto dmg = static_cast<int>(item);

		std::string text{ "$ hits $ for $ damage." };
		interpolate(text, user->name, enemy->name, dmg);
		auto msg = Message(text);
		add_message(msg);
		break;
	}
	}
}

void MessageLog::serialise(std::ofstream& file)
{
	auto num_messages = static_cast<uint32_t>(message_queue.size());
	utils::serialiseUint32(file, num_messages);
	utils::serialiseUint32(file, static_cast<uint32_t>(offset));
	std::for_each(message_queue.begin(), message_queue.end(), [&file](Message& m) {m.serialise(file); });
}

void MessageLog::deserialise(const char* buffer, size_t& _offset)
{
	auto num_messages = utils::deserialiseUint32(buffer, _offset);
	offset = utils::deserialiseUint32(buffer, _offset);
	for (int i = 0; i < num_messages; i++) {
		Message m;
		m.deserialise(buffer, _offset);
		message_queue.push_back(m);
	}
}

void MessageLog::load_intro(const char* path)
{
	std::ifstream file;
	std::string line;

	file.open(path);
	if (file.is_open()) {
		while (std::getline(file, line)) {
			auto m = Message(line);
			message_queue.push_back(m);
		}
	}
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

void Message::serialise(std::ofstream& file)
{
	utils::serialiseUint8(file, r);
	utils::serialiseUint8(file, g);
	utils::serialiseUint8(file, b);
	utils::serialiseString(file, text);
}

void Message::deserialise(const char* buffer, size_t& offset)
{
	r = utils::deserialiseUint8(buffer, offset);
	g = utils::deserialiseUint8(buffer, offset);
	b = utils::deserialiseUint8(buffer, offset);
	text = utils::deserialiseString(buffer, offset);
}
