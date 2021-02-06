#include "CharacterCreationScreen.hpp"

CharacterCreationScreen::CharacterCreationScreen(StateManager& _state_manager, World& _world, TextureManager& _tex_manager, EventManager& _event_manager, Keyboard& _keyboard):
	state_manager(_state_manager), world(_world), tex_manager(_tex_manager), event_manager(_event_manager), keyboard(_keyboard)
{
	stats = { "STR", "DEX", "CON", "WIS", "INT", "CHA" };

	std::string warrior_name = "Warrior";
	std::string warrior_desc = "A battle-hardened warrior who excels with close combat weapons.";
	std::vector<int> warrior_stats = { 12, 10, 12, 10, 8, 8 };
	char_options.insert({ 0, CharacterClass(warrior_name, warrior_desc, warrior_stats) });

	std::string wizard_name = "Wizard";
	std::string wizard_desc = "The wizard might look old and frail, but the space around them crackles with magical energy";
	std::vector<int> wizard_stats = { 8, 10, 8, 12, 12, 10 };
	char_options.insert({ 1, CharacterClass(wizard_name, wizard_desc, wizard_stats) });

	std::string archer_name = "Archer";
	std::string archer_desc = "You never much liked the idea of getting stabbed, so you decided to kill from afar.";
	std::vector<int> archer_stats = { 8, 12, 10, 12, 9, 9 };
	char_options.insert({ 2, CharacterClass(archer_name, archer_desc, archer_stats) });
}

void CharacterCreationScreen::handle_input(SDL_Event& event)
{
	SDL_PollEvent(&event);
	if (event.type == SDL_QUIT) {
		state_manager.stop_playing();
	}

	auto key = keyboard.handle_input(event);

	switch (key) {
	case SDLK_UP: {
		selection = selection - 1 < 0 ? static_cast<int>(char_options.size()) - 1 : selection - 1;
		break;
	}
	case SDLK_DOWN: {
		selection = selection + 1 > static_cast<int>(char_options.size()) - 1 ? 0 : selection + 1;
		break;
	}
	}
}

void CharacterCreationScreen::on_tick()
{

}

void CharacterCreationScreen::update(float dt)
{

}

void CharacterCreationScreen::draw_scene(Renderer& renderer, const uint32_t fps, float dt) const
{
	renderer.DrawCharacterSelectionScene(fps, char_options, selection, stats);
}

void CharacterCreationScreen::on_entrance(Renderer& renderer) const
{

}

void CharacterCreationScreen::on_bury() const
{

}


