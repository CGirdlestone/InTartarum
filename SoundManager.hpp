#pragma once

#include "Common.hpp"

using SmartMusic = std::unique_ptr<Mix_Music, decltype(&Mix_FreeMusic)>;
using SmartChunk = std::unique_ptr<Mix_Chunk, decltype(&Mix_FreeChunk)>;

class SoundManager
{
private:
	std::map<unsigned int, SmartMusic> music;
	std::map<std::string, unsigned int> music_map;
	unsigned int music_counter{ 0 };

	std::map<unsigned int, SmartChunk> chunks;
	std::map<std::string, unsigned int> chunk_map;
	unsigned int chunk_counter{ 0 };
public:
	SoundManager() {};
	~SoundManager() {};
	unsigned int LoadMusic(const std::string& path);
	unsigned int LoadChunk(const std::string& path);
	inline Mix_Music* GetMusic(const unsigned int index) const { return music.find(index) != music.end() ? music.at(index).get() : nullptr; };
	inline Mix_Chunk* GetChunk(const unsigned int index) const { return chunks.find(index) != chunks.end() ? chunks.at(index).get() : nullptr; };
};

