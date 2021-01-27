#include "SoundManager.hpp"

unsigned int SoundManager::LoadMusic(const std::string& path)
{
	if (music_map.find(path) != music_map.end()) {
		return music_map[path];
	}

	auto music_ptr = Mix_LoadMUS(path.c_str());
	if (music_ptr == nullptr) {
		exit(1); // must fix this...
	}

	auto id = music_counter++;
	SmartMusic smart_music(music_ptr, Mix_FreeMusic);
	music_map.insert({ path, id });
	music.insert({ id, std::move(smart_music) });

	return id;
}

unsigned int SoundManager::LoadChunk(const std::string& path)
{
	if (chunk_map.find(path) != chunk_map.end()) {
		return chunk_map[path];
	}

	auto chunk_ptr = Mix_LoadWAV(path.c_str());
	if (chunk_ptr == nullptr) {
		exit(1); // must fix this...
	}

	auto id = chunk_counter++;
	SmartChunk smart_chunk(chunk_ptr, Mix_FreeChunk);
	chunk_map.insert({ path, id });
	chunks.insert({ id, std::move(smart_chunk) });

	return id;
}
