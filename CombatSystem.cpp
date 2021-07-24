#include "CombatSystem.hpp"
#include "Utils.hpp"

CombatSystem::CombatSystem(World& _world, EventManager& _event_manager, WorldMap& _world_map)
	:world(_world), event_manager(_event_manager), world_map(_world_map)
{
	event_manager.add_subscriber(EventTypes::BUMP_ATTACK, *this);
	event_manager.add_subscriber(EventTypes::APPLY_DAMAGE, *this);
}

CombatSystem::~CombatSystem()
{

}

void CombatSystem::update(float dt) 
{

}

void CombatSystem::on_tick() 
{

}

void CombatSystem::receive(EventTypes event) 
{

}

void CombatSystem::receive(EventTypes event, uint32_t actor) 
{

}

void CombatSystem::receive(EventTypes event, uint32_t actor, uint32_t target) 
{
	switch (event) {
	case EventTypes::BUMP_ATTACK: attack(actor, target);
	}
}

void CombatSystem::receive(EventTypes event, uint32_t actor, uint32_t target, uint32_t item) 
{
	switch (event) {
	case EventTypes::APPLY_DAMAGE: apply_external_damage(actor, target, item);
	}
}

void CombatSystem::apply_external_damage(uint32_t actor, uint32_t target, uint32_t damage)
{
	auto* attacker = world.GetComponent<Fighter>(actor);
	auto* player_is_attacker = world.GetComponent<Player>(actor);

	auto* defender = world.GetComponent<Fighter>(target);
	auto* player_is_defender = world.GetComponent<Player>(target);

	defender->hp -= damage;
	event_manager.push_event(EventTypes::DEAL_DAMAGE, actor, target, static_cast<uint32_t>(damage));

	check_alive(actor, target);
}

void CombatSystem::attack(uint32_t actor, uint32_t target)
{
	auto* attacker = world.GetComponent<Fighter>(actor);
	auto* player_is_attacker = world.GetComponent<Player>(actor);

	auto* defender = world.GetComponent<Fighter>(target);
	auto* player_is_defender = world.GetComponent<Player>(target);

	if (defender == nullptr || attacker == nullptr) {
		return;
	}

	bool crit = true;

	if (try_hit(actor, target, crit)) {

		if (crit) {
			// do something here
		}

		std::string dmg_roll = "2d6";
		int dmg = utils::roll(dmg_roll) + attacker->str_mod;
		defender->hp -= dmg;
		event_manager.push_event(EventTypes::DEAL_DAMAGE, actor, target, static_cast<uint32_t>(dmg));

		check_alive(actor, target);
	}
}

bool CombatSystem::try_hit(uint32_t actor, uint32_t target, bool& crit)
{
	auto* attacker = world.GetComponent<Fighter>(actor);
	auto* defender = world.GetComponent<Fighter>(target);

	std::string attack_roll = "3d6";
	int score = utils::roll(attack_roll) + attacker->str_mod;
	int defence = 9 + defender->defence;

	if (score == 18) {
		crit = true;
		return true;
	} else if (score > defence) {
		return true;
	}
	else {
		return false;
	}
}

void CombatSystem::on_death(uint32_t actor) {
	auto* sprite = world.GetComponent<Sprite>(actor);
	if (sprite != nullptr) {
		sprite->clip_x = 5 * sprite->width;
		sprite->clip_y = 2 * sprite->height;
		sprite->r = 205;
		sprite->g = 92;
		sprite->b = 92;
	}
	
	auto* script = world.GetComponent<Scriptable>(actor);
	if (script != nullptr) {
		if (script->OnDeath != "") {
			event_manager.push_event(EventTypes::ON_DEATH_SCRIPT, actor);
		}
	}
}

void CombatSystem::check_alive(uint32_t actor, uint32_t target)
{
	auto* attacker = world.GetComponent<Fighter>(actor);
	auto* player_is_attacker = world.GetComponent<Player>(actor);

	auto* defender = world.GetComponent<Fighter>(target);
	auto* player_is_defender = world.GetComponent<Player>(target);

	if (defender->hp <= 0) {
		on_death(target);

		if (player_is_attacker != nullptr) {
			event_manager.push_event(EventTypes::MOB_DEATH, target);
			event_manager.push_event(EventTypes::EXP_GAIN, world.GetComponent<AI>(target)->xp);

			world.RemoveComponent<Fighter>(target);
			world.RemoveComponent<AI>(target);
			world.RemoveComponent<Actor>(target);
			world.RemoveComponent<Blocker>(target);
		}
		else {
			event_manager.push_event(EventTypes::PLAYER_DEATH, target);
		}
	}
}
