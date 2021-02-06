#pragma once

#include "Common.hpp"

class Window
{
private:
	std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> context;
	std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> renderer;
	int width;
	int height;
	int default_width;
	int default_height;
	int tile_width;
	int tile_height;

public:
	Window(const char* title, int _width, int _height, int _tile_width, int _tile_height);
	~Window();
	
	inline SDL_Renderer* GetRenderer() const { return renderer.get(); };
	inline const int GetWidth() const { return width; };
	inline const int GetHeight() const { return height; };
	inline const int GetTileWidth() const { return tile_height; };
	inline const int GetTileHeight() const { return tile_height; };
};

