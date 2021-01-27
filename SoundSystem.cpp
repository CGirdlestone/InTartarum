#include "SoundSystem.hpp"

SoundSystem::SoundSystem(EventManager& _event_manager, SoundManager& _sound_manager) 
	: event_manager(_event_manager), sound_manager(_sound_manager)
{
	event_manager.add_subscriber(EventTypes::PLAY_SOUND, *this);
	event_manager.add_subscriber(EventTypes::FADE_OUT_SOUND, *this);
	event_manager.add_subscriber(EventTypes::FADE_IN_SOUND, *this);
	event_manager.add_subscriber(EventTypes::PLAY_CHUNK, *this);
	event_manager.add_subscriber(EventTypes::BUTTON_CLICK, *this);
	event_manager.add_subscriber(EventTypes::STOP_MUSIC, *this);
}

void SoundSystem::play_chunks()
{
	while (queued_chunks.size() > 0) {
		auto chunk = queued_chunks.begin();
		Mix_PlayChannel(-1, sound_manager.GetChunk(*chunk), 0);
		queued_chunks.erase(chunk);
	}
}

void SoundSystem::update(float dt)
{
	cumulative_time += dt;
	if (cumulative_time > interval) {
		cumulative_time = 0.0f;
		play_chunks();
	}
}

void SoundSystem::on_tick()
{

}

void SoundSystem::receive(EventTypes event)
{
	switch (event) {
	case EventTypes::FADE_OUT_SOUND: Mix_FadeOutMusic, 500; break;
	case EventTypes::BUTTON_CLICK: queued_chunks.insert(sound_manager.LoadChunk("./Resources/Sounds/SFX/click3.wav")); break;
	}
}

void SoundSystem::receive(EventTypes event, uint32_t actor)
{
	switch (event) {
	case EventTypes::PLAY_SOUND: Mix_PlayMusic(sound_manager.GetMusic(actor), -1); break;
	case EventTypes::PLAY_CHUNK: queued_chunks.insert(actor); break;
	case EventTypes::FADE_IN_SOUND: Mix_FadeInMusic(sound_manager.GetMusic(actor), -1, 500); break;
	}
}

void SoundSystem::receive(EventTypes event, uint32_t actor, uint32_t target)
{

}

void SoundSystem::receive(EventTypes event, uint32_t actor, uint32_t target, uint32_t item)
{
	
}
