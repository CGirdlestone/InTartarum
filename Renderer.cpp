#include "Renderer.hpp"

Renderer::Renderer(World& _world, Window& _window, TextureManager& _tex_manager, Camera& _camera) :
	world(_world), window(_window), texture_manager(_tex_manager), camera(_camera), map_texture(nullptr, SDL_DestroyTexture)
{
	auto tex = SDL_CreateTexture(window.GetRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, camera.get_map_width() * window.GetTileWidth(), camera.get_map_height() * window.GetTileHeight());
	map_texture.reset(tex);
	LoadFont();
};

void Renderer::DrawMap(Level& level)
{
	auto& grid = level.get_grid();

	SetTarget(map_texture.get());
	Clear();

	for (int j = 0; j < camera.get_map_height(); j++) {
		for (int i = 0; i < camera.get_map_width(); i++) {
			auto& tile = grid.get_tile(i, j);

			SDL_Rect dstrect;
			dstrect.x = i * window.GetTileWidth();
			dstrect.y = j * window.GetTileHeight();
			dstrect.w = window.GetTileWidth();
			dstrect.h = window.GetTileHeight();

			auto sprite = tile_sprites.at(tile.type);

			if (!tile.explored) {
				sprite = tile_sprites.at(TileType::EMPTY);
			}

			SDL_Rect srcrect;
			srcrect.x = sprite.clip_x;
			srcrect.y = sprite.clip_y;
			srcrect.w = sprite.width;
			srcrect.h = sprite.height;


			SDL_SetTextureColorMod(texture_manager.GetTexture(sprite.id), sprite.r, sprite.g, sprite.b);
			SDL_RenderCopy(window.GetRenderer(), texture_manager.GetTexture(sprite.id), &srcrect, &dstrect);

			if (!tile.visible) {
				SDL_SetRenderDrawColor(window.GetRenderer(), 50, 50, 50, 128);
				SDL_SetRenderDrawBlendMode(window.GetRenderer(), SDL_BLENDMODE_BLEND);
				SDL_RenderFillRect(window.GetRenderer(), &dstrect);
				SDL_SetRenderDrawColor(window.GetRenderer(), window.GetBackground().r, window.GetBackground().g, window.GetBackground().b, 0);
			}
		}
	}
	ResetTarget();
}

void Renderer::DrawBox(int x, int y, int width, int height, bool lower_join, bool upper_join, bool game_border)
{
	for (int i = 0; i <= width; i++) {
		SDL_Rect rect;
		rect.x = (x + i) * window.GetTileWidth();
		rect.y = y * window.GetTileHeight();
		rect.w = window.GetTileWidth();
		rect.h = window.GetTileHeight();

		Sprite sprite;
		SDL_Rect srcrect;
		if (i == 0) {
			if (!upper_join) {
				sprite = tile_sprites.at(TileType::BORDER_TL);
				srcrect.x = sprite.clip_x;
				srcrect.y = sprite.clip_y;
			}
			else {
				sprite = tile_sprites.at(TileType::BORDER_LEFT_T);
				srcrect.x = sprite.clip_x;
				srcrect.y = sprite.clip_y;
			}
		}
		else if (i == width) {
			if (!upper_join){
				sprite = tile_sprites.at(TileType::BORDER_TR);
				srcrect.x = sprite.clip_x;
				srcrect.y = sprite.clip_y;
			}
			else {
				sprite = tile_sprites.at(TileType::BORDER_RIGHT_T);
				srcrect.x = sprite.clip_x;
				srcrect.y = sprite.clip_y;
			}
		}
		else {
			sprite = tile_sprites.at(TileType::BORDER_HORIZONTAL);
			srcrect.x = sprite.clip_x;
			srcrect.y = sprite.clip_y;
		}
		if (!game_border || (i == 0 || i == width)) {
			srcrect.w = sprite.width;
			srcrect.h = sprite.height;
			SDL_SetTextureColorMod(texture_manager.GetTexture(sprite.id), sprite.r, sprite.g, sprite.b);
			SDL_RenderCopy(window.GetRenderer(), texture_manager.GetTexture(sprite.id), &srcrect, &rect);
		}

		rect.y = (y + height) * window.GetTileHeight();
		if (i == 0) {
			if (!lower_join) {
				sprite = tile_sprites.at(TileType::BORDER_BL);
				srcrect.x = sprite.clip_x;
				srcrect.y = sprite.clip_y;
			}
			else {
				sprite = tile_sprites.at(TileType::BORDER_LEFT_T);
				srcrect.x = sprite.clip_x;
				srcrect.y = sprite.clip_y;
			}
		}
		else if (i == width) {
			if (!lower_join) {
				sprite = tile_sprites.at(TileType::BORDER_BR);
				srcrect.x = sprite.clip_x;
				srcrect.y = sprite.clip_y;
			}
			else {
				sprite = tile_sprites.at(TileType::BORDER_RIGHT_T);
				srcrect.x = sprite.clip_x;
				srcrect.y = sprite.clip_y;
			}
		}
		else {
			sprite = tile_sprites.at(TileType::BORDER_HORIZONTAL);
			srcrect.x = sprite.clip_x;
			srcrect.y = sprite.clip_y;
		}
		srcrect.w = sprite.width;
		srcrect.h = sprite.height;
		SDL_SetTextureColorMod(texture_manager.GetTexture(sprite.id), sprite.r, sprite.g, sprite.b);
		SDL_RenderCopy(window.GetRenderer(), texture_manager.GetTexture(sprite.id), &srcrect, &rect);
	}

	for (int j = 1; j < height; j++) {

		SDL_Rect rect;
		rect.x = x * GetTileWidth();
		rect.y = (y + j) * window.GetTileHeight();
		rect.w = window.GetTileWidth();
		rect.h = window.GetTileHeight();

		Sprite sprite;
		SDL_Rect srcrect;

		sprite = tile_sprites.at(TileType::BORDER_VERTICAL);
		srcrect.x = sprite.clip_x;
		srcrect.y = sprite.clip_y;
		srcrect.w = sprite.width;
		srcrect.h = sprite.height;

		SDL_SetTextureColorMod(texture_manager.GetTexture(sprite.id), sprite.r, sprite.g, sprite.b);
		SDL_RenderCopy(window.GetRenderer(), texture_manager.GetTexture(sprite.id), &srcrect, &rect);

		rect.x = (width + x) * GetTileWidth();
		SDL_SetTextureColorMod(texture_manager.GetTexture(sprite.id), sprite.r, sprite.g, sprite.b);
		SDL_RenderCopy(window.GetRenderer(), texture_manager.GetTexture(sprite.id), &srcrect, &rect);
	}
}

void Renderer::DrawMiniMap(int x, int y, int width, int height)
{
	SDL_Rect rect;
	rect.x = x * window.GetTileWidth();
	rect.y = y * window.GetTileHeight();
	rect.w = width * window.GetTileWidth();
	rect.h = height * window.GetTileHeight();

	auto empty_skill = texture_manager.GetTexture(texture_manager.LoadTexture("./Resources/UniqueBorderV11.png"));
	SDL_RenderCopy(window.GetRenderer(), empty_skill, nullptr, &rect);

	rect.x = x * window.GetTileWidth() + window.GetTileWidth() / 2;
	rect.y = y * window.GetTileHeight() + window.GetTileHeight() / 2;
	rect.w = width * window.GetTileWidth() - window.GetTileWidth();
	rect.h = height * window.GetTileHeight() - window.GetTileHeight();

	SDL_RenderCopy(window.GetRenderer(), map_texture.get(), nullptr, &rect);
}

void Renderer::DrawSkills()
{
	auto skill = texture_manager.GetTexture(texture_manager.LoadTexture("./Resources/Skills/fireball.png"));
	auto empty_skill = texture_manager.GetTexture(texture_manager.LoadTexture("./Resources/Skills/EmptyButton.png"));
	for (int i = 0; i < 9; i++) {
		SDL_Rect rect;
		rect.x = i * window.GetTileWidth() * 4;
		rect.y = (window.GetHeight() - 4) * window.GetTileHeight();
		rect.w = window.GetTileWidth() * 4;
		rect.h = window.GetTileHeight() * 4;

		if (i == 0) {
			SDL_RenderCopy(window.GetRenderer(), skill, nullptr, &rect);
		}
		else {
			SDL_RenderCopy(window.GetRenderer(), empty_skill, nullptr, &rect);
		}
	}

}

void Renderer::DrawText(const std::string& text, int x, int y, uint8_t r, uint8_t g, uint8_t b)
{
	auto x_start{ x * window.GetTileWidth() };
	auto offset{ 0 };
	SDL_SetTextureColorMod(texture_manager.GetTexture(font_id), r, g, b);
	for (char c : text) {
		SDL_Rect dstrect;
		dstrect.x = x_start + (offset++) * font_width;
		dstrect.y = y * window.GetTileHeight();
		dstrect.w = font_width;
		dstrect.h = font_height;
		
		SDL_Rect srcrect;
		srcrect.x = (static_cast<int>(c) % 16) * font_width;
		srcrect.y = (static_cast<int>(c) / 16) * font_height;
		srcrect.w = font_width;
		srcrect.h = font_height;

		SDL_RenderCopy(window.GetRenderer(), texture_manager.GetTexture(font_id), &srcrect, &dstrect);
	}
}

void Renderer::DrawMessageLog(MessageLog& message_log)
{
	auto& messages = message_log.get_messages();
	auto it = messages.rbegin();
	int j{ camera.get_height() + 2 * camera.get_offset_y() };
	for (int i = 0; i < 10; i++) {
		const auto& msg = *(it + message_log.get_offset() + i);
		auto & [r, g, b] = msg.get_colour();
		DrawText(msg.text, camera.get_offset_x() * camera.get_zoom() , j++, r, g, b);
		if ((it + message_log.get_offset()) + i + 1 == messages.rend()) {
			break;
		}
	}
}

void Renderer::LoadFont()
{
	SmartLuaVM vm(nullptr, &lua_close);
	vm.reset(luaL_newstate());
	auto result = luaL_dofile(vm.get(), "./Config/window.lua");
	std::string _font{ "" };

	if (result == LUA_OK) {
		lua_getglobal(vm.get(), "font_path");
		if (lua_isstring(vm.get(), -1)) {
			_font = std::string(lua_tostring(vm.get(), -1));
		}
		lua_getglobal(vm.get(), "font_width");
		if (lua_isnumber(vm.get(), -1)) {
			font_width = static_cast<int>(lua_tonumber(vm.get(), -1));
		}
		lua_getglobal(vm.get(), "font_height");
		if (lua_isnumber(vm.get(), -1)) {
			font_height = static_cast<int>(lua_tonumber(vm.get(), -1));
		}
	}
	else {
		std::string error_msg = lua_tostring(vm.get(), -1);
		printf(error_msg.c_str());
	}

	std::string font = "./Resources/" + _font;
	font_id = texture_manager.LoadTexture(font, true);
}

std::vector<std::string> Renderer::WrapText(const std::string& text, int line_width)
{
	std::vector<std::string> lines;
	int last_space{ 0 };
	int line_start{ 0 };

	// no need to wrap so push single line and return 
	if (text.length() < line_width) {
		lines.push_back(text);
		return lines;
	}

	for (uint32_t i = 0; i < text.length(); ++i) {
		if (text[i] == ' ') {
			last_space = i;
		}

		if (i == text.length() - 1) {
			lines.push_back(text.substr(line_start + 1, i + 1 - line_start));
			break;
		}

		if (i % line_width == line_width - 1) {
			if (line_start == 0) {
				lines.push_back(text.substr(line_start, last_space - line_start));
			}
			else {
				lines.push_back(text.substr(line_start + 1, last_space - line_start));
			}
			line_start = last_space;
		}
	}

	return lines;
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
	dstrect.x = x * window.GetTileWidth();
	dstrect.y = y * window.GetTileHeight();
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
	auto [cam_x, cam_y] = camera.get_position();
	dstrect.x = (x * camera.get_zoom() + camera.get_offset_x()) * window.GetTileWidth();
	dstrect.y = (y * camera.get_zoom() + camera.get_offset_y()) * window.GetTileHeight();
	dstrect.w = sprite->width * camera.get_zoom();
	dstrect.h = sprite->height * camera.get_zoom();
	
	SDL_Rect srcrect;
	srcrect.x = sprite->clip_x;
	srcrect.y = sprite->clip_y;
	srcrect.w = sprite->width;
	srcrect.h = sprite->height;

	SDL_SetTextureColorMod(texture_manager.GetTexture(sprite->id), sprite->r, sprite->g, sprite->b);
	SDL_RenderCopy(window.GetRenderer(), texture_manager.GetTexture(sprite->id), &srcrect, &dstrect);
}

void Renderer::DrawSplash(unsigned int tex_id, const uint32_t fps, float dt, int option, bool save_game)
{
	SDL_SetTextureColorMod(texture_manager.GetTexture(tex_id), 128, 0, 0);
	SDL_RenderCopy(window.GetRenderer(), texture_manager.GetTexture(tex_id), nullptr, nullptr);

	auto* title = texture_manager.GetTexture(texture_manager.LoadTexture("./Resources/In_Tartarum.png"));
	SDL_Rect dstrect;
	SDL_QueryTexture(title, nullptr, nullptr, &dstrect.w, &dstrect.h);
	dstrect.x = (window.GetWidth() * window.GetTileWidth()) / 2 - dstrect.w / 2;
	dstrect.y = window.GetTileHeight() * 4;
	SDL_RenderCopy(window.GetRenderer(), title, nullptr, &dstrect);

	auto y{ window.GetTileHeight() * (window.GetHeight() - 4) / 2 };
	auto* continue_game = texture_manager.GetTexture(texture_manager.LoadTexture("./Resources/Continue_text.png"));
	SDL_Rect continue_rect;
	SDL_QueryTexture(continue_game, nullptr, nullptr, &continue_rect.w, &continue_rect.h);
	auto x{ window.GetWidth() / 2 * window.GetTileWidth() - continue_rect.w / 2 };
	continue_rect.x = x;
	continue_rect.y = y;

	if (save_game) {
		y += window.GetTileHeight() * 2;
		SDL_RenderCopy(window.GetRenderer(), continue_game, nullptr, &continue_rect);
	}

	auto* new_game = texture_manager.GetTexture(texture_manager.LoadTexture("./Resources/New_game_text.png"));
	SDL_Rect new_game_rect;
	SDL_QueryTexture(new_game, nullptr, nullptr, &new_game_rect.w, &new_game_rect.h);
	new_game_rect.x = x;
	new_game_rect.y = y;
	y += window.GetTileHeight() * 2;
	SDL_RenderCopy(window.GetRenderer(), new_game, nullptr, &new_game_rect);

	auto* quit_game = texture_manager.GetTexture(texture_manager.LoadTexture("./Resources/Quit_text.png"));
	SDL_Rect quit_rect;
	SDL_QueryTexture(quit_game, nullptr, nullptr, &quit_rect.w, &quit_rect.h);
	quit_rect.x = x;
	quit_rect.y = y;
	SDL_RenderCopy(window.GetRenderer(), quit_game, nullptr, &quit_rect);

	auto* cursor = texture_manager.GetTexture(texture_manager.LoadTexture("./Resources/Cursor.png"));
	SDL_Rect cursor_rect;
	SDL_QueryTexture(cursor, nullptr, nullptr, &cursor_rect.w, &cursor_rect.h);
	cursor_rect.x = x - window.GetTileWidth();
	cursor_rect.y = (window.GetTileHeight() * (window.GetHeight() - 4) / 2) + window.GetTileHeight() * 2 * option;
	SDL_RenderCopy(window.GetRenderer(), cursor, nullptr, &cursor_rect);

	DrawFPS(fps);
}

void Renderer::DrawCharacterSelectionScene(const uint32_t fps, const std::map<int, CharacterClass>& character_options, int selected, const std::vector<std::string>& stats)
{
	int num_options{ static_cast<int>(character_options.size()) };
	DrawBox(1, 1, 10, 20);

	//int y{ 0 };
	//for (auto& [id, character] : character_options) {
	//	if (y == selected) {
	//		DrawText(character.name, 2, 2 * (y++ + 1), 0xE8, 0xAE, 0x5B);
	//	}
	//	else {
	//		DrawText(character.name, 2, 2 * (y++ + 1), 0xBB, 0xAA, 0x99);
	//	}
	//}

	//auto description = WrapText(character_options.at(selected).description, 30);

	//int x{ 12 };
	//y = 1;
	//j = 2 * num_options + 1 + 2;
	//DrawBox(x, y, 22, 11);

	//std::for_each(description.cbegin(), description.cend(), [&y, this](const std::string& line) {this->DrawText(line, 13, 1 + y++, 0xBB, 0xAA, 0x99); });

	//y = 7;
	//int i{ 12 };
	//for (size_t k = 0; k < stats.size(); k++) {
	//	DrawText(stats[k], i + 1, y + 1, 0, 0, 0);
	//	DrawText(std::to_string(character_options.at(selected).stats[k]), i + 1, y + 3, 0xBB, 0xAA, 0x99);
	//	i += 3;
	//}
}

void Renderer::DrawScene(uint32_t fps, WorldMap& world_map, MessageLog& message_log)
{
	DrawMapTexture(camera.get_offset_x(), camera.get_offset_y());

	auto components = world.GetComponents<Position, Sprite>();
	// remove the entities that are not on this depth level.
	components.erase(std::remove_if(components.begin(), components.end(), [&world_map](const std::tuple<Position*, Sprite*>& a) {
		return std::get<0>(a)->z != world_map.get_current_depth();
		}
	), components.end());

	// sort the remaining entities based on the sprite depth value.
	std::sort(components.begin(), components.end(), [](const std::tuple<Position*, Sprite*>& a, const std::tuple<Position*, Sprite*>& b) {
		return std::get<1>(a)->depth < std::get<1>(b)->depth;
		}
	);

	auto& grid = world_map.get_level().get_grid();
	for (auto& [pos, sprite] : components) {
		if (grid.get_tile(pos->x, pos->y).visible) {
			DrawSprite(pos, sprite);
		}
	}

	DrawFPS(fps);
	DrawBox(0, 0, camera.get_width() + camera.get_offset_x(), camera.get_height() + camera.get_offset_y(), true, false, true);
	std::string depth = "Dungeon depth: " + std::to_string(world_map.get_current_depth());
	DrawText(depth, camera.get_offset_x() * camera.get_zoom(), 0, 0xBB, 0xAA, 0x99);

	if (!message_log.get_messages().empty()){
		DrawMessageLog(message_log);

	}
}


