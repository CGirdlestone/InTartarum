#include "TextureManager.hpp"

unsigned int TextureManager::LoadTexture(const std::string& texture_path, bool set_colour_key)
{
	/* Loads a texture in, checking for whether it has already been loaded. */

	// Check whether the texture has been loaded previously.
	// If it has, return the id.
	if (path_map.find(texture_path) != path_map.end()) {
		return path_map[texture_path];
	}

	// Load the image into a temporary surface.
	auto surface = IMG_Load(texture_path.c_str());
	if (surface == nullptr) {
		exit(1); // fix error handling
	}

	// If the colour key is to be set, make the colour of the (0,0) pixel transparent.
	if (set_colour_key) {
		uint32_t* pixels = static_cast<uint32_t*>(surface->pixels); 
		SDL_SetColorKey(surface, SDL_TRUE, pixels[0]);
	}

	// Convert the temporary surface to a texture
	auto texture = SDL_CreateTextureFromSurface(window.GetRenderer(), surface);
	if (texture == nullptr) {
		exit(1);
	}
	SDL_FreeSurface(surface);

	auto id = texture_counter++;
	SmartTexture smart_tex(texture, SDL_DestroyTexture);
	path_map.insert({ texture_path, id });
	textures.insert({ id, std::move(smart_tex) });

	return id;
}
