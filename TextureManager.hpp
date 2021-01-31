#pragma once

#include "Common.hpp"
#include "Window.hpp"

using SmartTexture = std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>;

class TextureManager
{
private:
	std::map<unsigned int, SmartTexture> textures;
	std::map<std::string, unsigned int> path_map;
	unsigned int texture_counter{ 0 };
	Window& window;
public:
	TextureManager(Window & _window):window(_window) {};
	~TextureManager() {};
	unsigned int LoadTexture(const std::string & texture_path, bool set_colour_key = false);
	inline SDL_Texture* GetTexture(const unsigned int index) const { return textures.find(index) != textures.end() ? textures.at(index).get() : nullptr; };
};

