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

struct Message
{
	Message(std::string _text);
	Message(std::string _text, uint8_t _r, uint8_t _g, uint8_t _b) : text(_text), r(_r), g(_g), b(_b) { };
	std::string text{ "" };
	uint8_t r{ 255 };
	uint8_t g{ 255 };
	uint8_t b{ 255 };
};

class MessageLog : public BaseSystem
{
public:
	MessageLog(World& _world, EventManager& _event_manager, int _width, int _height) 
		: world(_world), event_manager(_event_manager), randomiser(), width(_width), height(_height) { };
	~MessageLog() { };
	
	inline const std::vector<Message>& get_messages() const { return message_queue; };

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
	std::vector<Message> message_queue;
	RandomNumber randomiser;
	std::map<MessageTopic, std::vector<std::string> > descriptions;
	int width{ 0 };
	int height{ 0 };
	int message_history{ 100 };
	int offset{ 0 };

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

