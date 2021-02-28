#pragma once

#include "BaseSystem.hpp"
#include "EventManager.hpp"
#include "World.hpp"
#include "RandomNumberGenerator.hpp"

enum class MessageTopic {
	MELEE_ATTACK,
	RANGED_ATTACK,
	DEATH,
};

struct Message : public ISerializeable
{
	Message() {};
	Message(std::string _text) : text(_text) { };
	Message(const char* _text) : text(_text) { };
	Message(std::string _text, uint8_t _r, uint8_t _g, uint8_t _b) : text(_text), r(_r), g(_g), b(_b) { };
	const std::tuple<uint8_t, uint8_t, uint8_t> get_colour() const { return std::make_tuple(r, g, b); };
	std::string text{ "" };
	uint8_t r{ 0xBB };
	uint8_t g{ 0xAA };
	uint8_t b{ 0x99 };

	virtual void serialise(std::ofstream& file) override;
	virtual void deserialise(const char* buffer, size_t& offset) override;
};

class MessageLog : public BaseSystem
{
public:
	MessageLog(World& _world, EventManager& _event_manager);
	~MessageLog() { };
	
	inline const std::deque<Message>& get_messages() const { return message_queue; };
	inline const int get_offset() const { return offset; };
	inline void scroll_up() { offset = std::max(0, offset - 1); };
	inline void scroll_down() { offset = static_cast<int>(message_queue.size()) > num_lines ? std::min(static_cast<int>(message_queue.size()) - num_lines, offset + 1) : offset; };
	virtual void update(float dt) override;
	virtual void on_tick() override;	
	virtual void receive(EventTypes event) override;
	virtual void receive(EventTypes event, uint32_t actor) override;
	virtual void receive(EventTypes event, uint32_t actor, uint32_t target) override;
	virtual void receive(EventTypes event, uint32_t actor, uint32_t target, uint32_t item) override;
	void serialise(std::ofstream& file);
	void deserialise(const char* buffer, size_t& offset);

private:
	World& world;
	EventManager& event_manager;
	std::deque<Message> message_queue;
	RandomNumber randomiser;
	std::map<MessageTopic, std::vector<std::string> > descriptions;
	int width{ 0 };
	int height{ 0 };
	uint32_t message_history{ 0 };
	int offset{ 0 };
	int num_lines{ 0 };

	void load_descriptions(const char* path);
	void add_message(Message& message);

	inline void interpolate(std::string& flavour_text, int x) {
		flavour_text.replace(flavour_text.find('$'), 1, std::to_string(x));
	};

	inline void interpolate(std::string& flavour_text, std::string s) {
		flavour_text.replace(flavour_text.find('$'), 1, s);
	};

	template<typename T, typename... Args>
	void interpolate(std::string& flavour_text, T t, Args... args) {
		interpolate(flavour_text, t);
		interpolate(flavour_text, args...);
	};
};

