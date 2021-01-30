#include "Renderer.hpp"

Renderer::Renderer(World& _world, Window& _window, TextureManager& _tex_manager, Camera& _camera) :
	world(_world), window(_window), texture_manager(_tex_manager), camera(_camera), map_texture(nullptr, SDL_DestroyTexture)
{
	auto tex = SDL_CreateTexture(window.GetRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, camera.get_map_width() * window.GetTileWidth(), camera.get_map_height() * window.GetTileHeight());
	map_texture.reset(tex);
};

void Renderer::DrawMap(Level& level)
{
	auto& grid = level.get_grid();

	SetTarget(map_texture.get());
	Clear();

	for (int j = 0; j < camera.get_map_height(); j++) {
		for (int i = 0; i < camera.get_map_width(); i++) {
			
			SDL_Rect dstrect;
			dstrect.x = i * window.GetTileWidth();
			dstrect.y = j * window.GetTileHeight();
			dstrect.w = window.GetTileWidth();
			dstrect.h = window.GetTileHeight();

			auto sprite = tile_sprites.at(TileType::EMPTY);

			SDL_Rect srcrect;
			srcrect.x = sprite.clip_x;
			srcrect.y = sprite.clip_y;
			srcrect.w = sprite.width;
			srcrect.h = sprite.height;

			SDL_RenderCopy(window.GetRenderer(), texture_manager.GetTexture(sprite.id), &srcrect, &dstrect);

			auto& tile = grid.get_tile(i, j);

			sprite = tile_sprites.at(tile.type);

			if (!tile.explored) {
				sprite = tile_sprites.at(TileType::EMPTY);
			}

			srcrect.x = sprite.clip_x;
			srcrect.y = sprite.clip_y;
			srcrect.w = sprite.width;
			srcrect.h = sprite.height;

			SDL_RenderCopy(window.GetRenderer(), texture_manager.GetTexture(sprite.id), &srcrect, &dstrect);

			if (!tile.visible) {
				SDL_SetRenderDrawColor(window.GetRenderer(), 50, 50, 50, 128);
				SDL_SetRenderDrawBlendMode(window.GetRenderer(), SDL_BLENDMODE_BLEND);
				SDL_RenderFillRect(window.GetRenderer(), &dstrect);
				SDL_SetRenderDrawColor(window.GetRenderer(), 255, 255, 255, 0);
			}
		}
	}
	ResetTarget();
}

void Renderer::DrawBox(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
	auto* gui_0 = texture_manager.GetTexture(texture_manager.LoadTexture("./Resources/GUI0.png"));

	for (unsigned int i = 0; i < width; i++) {
		for (unsigned int j = 0; j < height; j++) {
			SDL_Rect dstrect;
			dstrect.x = (x + i) * window.GetTileWidth();
			dstrect.y = (y + j) * window.GetTileHeight();
			dstrect.w = window.GetTileWidth();
			dstrect.h = window.GetTileHeight();
			unsigned int _x{ 2 };
			unsigned int _y{ 8 };

			if (i == 0 && j == 0) {
				// top left
				_x = 1;
				_y = 7;
			}
			else if (i == width - 1 && j == 0) {
				// top right
				_x = 3;
				_y = 7;
			}
			else if (i == 0 && j == height - 1) {
				// bottom left
				_x = 1;
				_y = 9;
			}
			else if (i == width - 1 && j == height - 1) {
				// bottom right
				_x = 3;
				_y = 9;
			}
			else if (j == 0) {
				// top
				_x = 2;
				_y = 7;
			}
			else if (j == height - 1) {
				// bottom
				_x = 2;
				_y = 9;
			}
			else if (i == 0) {
				// left
				_x = 1;
				_y = 8;
			}
			else if (i == width - 1) {
				// right
				_x = 3;
				_y = 8;
			}

			SDL_Rect srcrect;
			srcrect.x = _x * window.GetTileWidth();
			srcrect.y = _y * window.GetTileHeight();
			srcrect.w = window.GetTileWidth();
			srcrect.h = window.GetTileHeight();

			SDL_RenderCopy(window.GetRenderer(), gui_0, &srcrect, &dstrect);
		}
	}
}

void Renderer::DrawFPS(uint32_t fps)
{
	std::string fps_str = std::to_string(fps);

	for (unsigned int i = 0; i < fps_str.size(); i++) {
		SDL_Rect dstrect;
		dstrect.x = (i + 1) * 16;
		dstrect.y = 2 * 16;
		dstrect.w = 16;
		dstrect.h = 16;

		int x = static_cast<int>(fps_str[i]);

		SDL_Rect srcrect;
		srcrect.x = (36 + (x - 49)) * 16;
		srcrect.y = 17 * 16;
		srcrect.w = 16;
		srcrect.h = 16;
		SDL_RenderCopy(window.GetRenderer(), texture_manager.GetTexture(texture_manager.LoadTexture("./Resources/colored_packed.png")), &srcrect, &dstrect);
	}
}

void Renderer::DrawMapTexture(int x, int y)
{
	
	SDL_Rect dstrect;
	dstrect.x = 0;
	dstrect.y = 0;
	dstrect.w = camera.get_width() * window.GetTileWidth();
	dstrect.h = camera.get_height() * window.GetTileHeight();

	auto [cam_x, cam_y] = camera.get_position();
	SDL_Rect srcrect;
	srcrect.x = cam_x * window.GetTileWidth();
	srcrect.y = cam_y * window.GetTileHeight();;
	srcrect.w = camera.get_width() * window.GetTileWidth() / camera.get_zoom();
	srcrect.h = camera.get_height() * window.GetTileHeight() / camera.get_zoom();

	SDL_RenderCopy(window.GetRenderer(), map_texture.get(), &srcrect, &dstrect);
}


void Renderer::DrawSprite(Position* pos, Sprite* sprite)
{
	SDL_Rect dstrect;
	auto [x, y] = camera.viewport(pos->x, pos->y);
	dstrect.x = x * camera.get_zoom() * window.GetTileWidth();
	dstrect.y = y * camera.get_zoom() * window.GetTileHeight();
	dstrect.w = sprite->width * 2;
	dstrect.h = sprite->height * 2;

	SDL_Rect srcrect;
	srcrect.x = sprite->clip_x;
	srcrect.y = sprite->clip_y;
	srcrect.w = sprite->width;
	srcrect.h = sprite->height;

	SDL_RenderCopy(window.GetRenderer(), texture_manager.GetTexture(sprite->id), &srcrect, &dstrect);
}

void Renderer::DrawSplash(unsigned int tex_id, const uint32_t fps)
{
	Clear();
	SDL_RenderCopy(window.GetRenderer(), texture_manager.GetTexture(tex_id), nullptr, nullptr);
	DrawFPS(fps);
	Update();
}

void Renderer::DrawScene(uint32_t fps, WorldMap& world_map)
{
	Clear();
	
	DrawMapTexture(0, 0);
	DrawBox(camera.get_width(), 0, window.GetWidth() - camera.get_width(), window.GetHeight());

	auto components = world.GetComponents<Position, Sprite>();
	// sort the entities based on the sprite depth value.
	std::sort(components.begin(), components.end(), [](const std::tuple<Position*, Sprite*>& a, const std::tuple<Position*, Sprite*>& b) {
		return std::get<1>(a)->depth < std::get<1>(b)->depth;
		}
	);

	// only draw those that are on the same dungeon depth.
	components.erase(std::remove_if(components.begin(), components.end(), [&world_map](const std::tuple<Position*, Sprite*>& a) {
		return std::get<0>(a)->z != world_map.get_current_depth();
		}
	), components.end());
	auto& grid = world_map.get_level(world_map.get_current_depth()).get_grid();
	for (auto& [pos, sprite] : components) {
		if (grid.get_tile(pos->x, pos->y).visible) {
			DrawSprite(pos, sprite);
		}
	}

	DrawFPS(fps);
	
	Update();
}


