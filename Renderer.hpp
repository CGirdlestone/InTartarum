#pragma once

#include "World.hpp"
#include "Components.hpp"
#include "Window.hpp"
#include "Tile.hpp"
#include "TextureManager.hpp"
#include "Camera.hpp"
#include "WorldMap.hpp"

class Renderer
{
private:
	World& world;
	Window& window;
	TextureManager& texture_manager;
	Camera& camera;
	SmartTexture map_texture;
	std::map<TileType, Sprite> tile_sprites;

	void Update() { SDL_RenderPresent(window.GetRenderer()); };
	void Clear() { SDL_RenderClear(window.GetRenderer()); };

	inline void SetTarget(SDL_Texture* target) const { SDL_SetRenderTarget(window.GetRenderer(), target); };
	inline void ResetTarget() const { SDL_SetRenderTarget(window.GetRenderer(), nullptr); };
	void DrawMapTexture(int x, int y);
	void DrawFPS(uint32_t fps);
	void DrawSprite(Position* pos, Sprite* sprite);
	void DrawBox(unsigned int x, unsigned int y, unsigned int width, unsigned int height);
	void DrawMiniMap(unsigned int x, unsigned int y, unsigned int width, unsigned int height);
	void DrawSkills();
public:
	Renderer(World& _world, Window& _window, TextureManager& _tex_manager, Camera& _camera);
	virtual ~Renderer() {};
	inline void AddTile(TileType type, Sprite tile) { tile_sprites.insert({ type, tile }); };

	void DrawScene(uint32_t fps, WorldMap& world_map);
	void DrawSplash(unsigned int tex_id, const uint32_t fps, float dt);
	void DrawMap(Level& level);
};

