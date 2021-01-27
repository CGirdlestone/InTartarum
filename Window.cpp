#include "Window.hpp"

Window::Window(const char* title, unsigned int _width, unsigned int _height, unsigned int _tile_size) :
	context(nullptr, SDL_DestroyWindow), renderer(nullptr, SDL_DestroyRenderer),
	width(_width), height(_height), tile_size(_tile_size)
{
	context.reset(SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _width * _tile_size, _height * _tile_size, SDL_WINDOW_SHOWN));
	renderer.reset(SDL_CreateRenderer(context.get(), -1, SDL_RENDERER_ACCELERATED));

	SDL_SetRenderDrawColor(renderer.get(), 255, 255, 255, 0);
}

Window::~Window()
{

}

