#pragma once

#include "Common.hpp"
#include "Utils.hpp"

class Window
{
private:
	std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> context;
	std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> renderer;
	std::string title{ "" };
	int width{ 0 };
	int height{ 0 };
	int default_width{ 0 };
	int default_height{ 0 };
	int tile_width{ 0 };
	int tile_height{ 0 };
	SDL_Color background;

public:
	Window();
	~Window();
	
	inline SDL_Renderer* GetRenderer() const { return renderer.get(); };
	inline const int GetWidth() const { return width; };
	inline const int GetHeight() const { return height; };
	inline const int GetTileWidth() const { return tile_width; };
	inline const int GetTileHeight() const { return tile_height; };
	inline const SDL_Color& GetBackground() const { return background; };
};

