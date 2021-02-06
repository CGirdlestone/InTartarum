#include "Window.hpp"

Window::Window(const char* title, int _width, int _height, int _tile_width, int _tile_height) :
	context(nullptr, SDL_DestroyWindow), renderer(nullptr, SDL_DestroyRenderer),
	width(_width), height(_height), default_width(_width), default_height(_height), tile_width(_tile_width), tile_height(_tile_height)
{
	context.reset(SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _width * _tile_width, _height * _tile_height, SDL_WINDOW_SHOWN));
	renderer.reset(SDL_CreateRenderer(context.get(), -1, SDL_RENDERER_ACCELERATED));

	SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 0);
}

Window::~Window()
{

}

