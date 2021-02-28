#pragma once

#include "World.hpp"
#include "Components.hpp"
#include "Window.hpp"
#include "Tile.hpp"
#include "TextureManager.hpp"
#include "Camera.hpp"
#include "WorldMap.hpp"
#include "CharacterClass.hpp"
#include "MessageLog.hpp"


class Renderer
{
private:
	World& world;
	Window& window;
	TextureManager& texture_manager;
	Camera& camera;
	SmartTexture map_texture;
	std::map<TileType, Sprite> tile_sprites;

	unsigned int font_id{ 0 };
	int font_width{ 0 };
	int font_height{ 0 };

	inline void SetTarget(SDL_Texture* target) const { SDL_SetRenderTarget(window.GetRenderer(), target); };
	inline void ResetTarget() const { SDL_SetRenderTarget(window.GetRenderer(), nullptr); };
	void DrawMapTexture(int x, int y);
	void DrawFPS(uint32_t fps);
	void DrawSprite(Position* pos, Sprite* sprite);
	void DrawBox(int x, int y, int width, int height, bool lower_join=false, bool upper_join=false, bool game_border=false);
	void DrawMiniMap(int x, int y, int width, int height);
	void DrawSkills();
	void DrawText(const std::string& text, int x, int y, uint8_t r = 0, uint8_t g = 0, uint8_t b = 0);
	void DrawMessageLog(MessageLog& message_log);
	void DrawGameBorder(int x, int y, int width, int height, int height_divider);
	void DrawHealth(int x, int y, int health, int max_health);
	void DrawPlayerInfo(WorldMap& world_map);
	void DrawBackgroundTile(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF, bool scale = false);
	void LoadFont();
	std::vector<std::string> WrapText(const std::string& text, int line_width);
public:
	Renderer(World& _world, Window& _window, TextureManager& _tex_manager, Camera& _camera);
	virtual ~Renderer() {};
	inline void AddTile(TileType type, Sprite tile) { tile_sprites.insert({ type, tile }); };
	inline void Update() { SDL_RenderPresent(window.GetRenderer()); };
	inline void Clear() { SDL_RenderClear(window.GetRenderer()); };
	inline const int GetTileWidth() const { return window.GetTileWidth(); };
	inline const int GetTileHeight() const { return window.GetTileHeight(); };
	void DrawScene(const uint32_t fps, WorldMap& world_map, MessageLog& message_log);
	void DrawSplash(unsigned int tex_id, const uint32_t fps, float dt, int option, bool save_game);
	void DrawCharacterSelectionScene(const uint32_t, const std::map<int, CharacterClass>& character_options, int selected, const std::vector<std::string>& stats);
	void DrawMap(Level& level);
	void DrawInventory(const std::vector<uint32_t>& items, const std::vector<std::string>& equipment_slots, int option, bool in_equipment_list);
	void DrawActions(bool in_equipment_list);
	void DrawQuantity(const std::string& quantity);
	void DrawLook(int x, int y);
	void DrawTargeting(int x, int y, const std::vector<std::tuple<int, int> >& path, int range, int aoe);
	void DrawHelp();
	void SetFont(unsigned int id, int width, int height) { font_id = id; font_width = width; font_height = height; };
};