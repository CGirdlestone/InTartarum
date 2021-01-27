#include "TextureManager.hpp"

unsigned int TextureManager::LoadTexture(const std::string& texture_path)
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
