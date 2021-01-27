#pragma once

#include "Common.hpp"

class Window
{
private:
	std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> context;
	std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> renderer;
	unsigned int width;
	unsigned int height;
	unsigned int tile_size;

public:
	Window(const char* title, unsigned int _width, unsigned int _height, unsigned int _tile_size);
	~Window();
	
	inline SDL_Renderer* GetRenderer() const { return renderer.get(); };
	inline const unsigned int GetWidth() const { return width; };
	inline const unsigned int GetHeight() const { return height; };
	inline const unsigned int GetTileSize() const { return tile_size; };
};

