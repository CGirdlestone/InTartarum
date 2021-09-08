#pragma once

#include "SoundManager.hpp"
#include "EventManager.hpp"

class SoundSystem : public BaseSystem
{
private:
	EventManager& event_manager;
	SoundManager& sound_manager;
	std::set<uint32_t> queued_chunks;
	float interval{ 0.005f };
	float cumulative_time{ 0.0f };
	void play_chunks();

	std::string sfx_path{ "./Resources/Sounds/SFX" };
	std::string music_path{ "./Resources/Sounds" };
	
public:
	SoundSystem(EventManager& _event_manager, SoundManager& _sound_manager);
	~SoundSystem() { };

	void play_music(std::string& music);
	void play_chunk(std::string& chunk);
	void fade_in(std::string& music, int ms);
	void fade_out(int ms);

	virtual void update(float dt) override;
	virtual void on_tick() override;
	virtual void receive(EventTypes event) override;
	virtual void receive(EventTypes event, uint32_t actor) override;
	virtual void receive(EventTypes event, uint32_t actor, uint32_t target) override;
	virtual void receive(EventTypes event, uint32_t actor, uint32_t target, uint32_t item) override;
};

