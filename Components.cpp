#include "Components.hpp"

void Position::serialise(std::ofstream& file)
{
	utils::serialiseUint32(file, static_cast<uint32_t>(x));
	utils::serialiseUint32(file, static_cast<uint32_t>(y));
	utils::serialiseUint32(file, static_cast<uint32_t>(z));
	utils::serialiseUint32(file, static_cast<uint32_t>(world_x));
	utils::serialiseUint32(file, static_cast<uint32_t>(world_y));
}

void Position::deserialise(const char* buffer, size_t& offset)
{
	x = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	y = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	z = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	world_x = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	world_y = static_cast<int>(utils::deserialiseUint32(buffer, offset));
}

void Player::serialise(std::ofstream& file)
{
	utils::serialiseUint32(file, static_cast<uint32_t>(level));
	utils::serialiseUint32(file, static_cast<uint32_t>(vision));
	utils::serialiseUint32(file, xp);
	utils::serialiseUint32(file, next_xp);
}

void Player::deserialise(const char* buffer, size_t& offset)
{
	level = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	vision = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	xp = utils::deserialiseUint32(buffer, offset);
	next_xp = utils::deserialiseUint32(buffer, offset);
}

void Actor::serialise(std::ofstream& file)
{
	utils::serialiseString(file, name);
	utils::serialiseString(file, description);
}

void Actor::deserialise(const char* buffer, size_t& offset)
{
	name = utils::deserialiseString(buffer, offset);
	description = utils::deserialiseString(buffer, offset);
}

void Blocker::serialise(std::ofstream& file)
{
	utils::serialiseUint32(file, static_cast<uint32_t>(blocks_view));
}

void Blocker::deserialise(const char* buffer, size_t& offset)
{
	blocks_view = static_cast<bool>(utils::deserialiseUint32(buffer, offset));
}

void LightSource::serialise(std::ofstream& file)
{
	utils::serialiseUint32(file, static_cast<uint32_t>(radius));
}

void LightSource::deserialise(const char* buffer, size_t& offset)
{
	radius = static_cast<int>(utils::deserialiseUint32(buffer, offset));
}

void Interactable::serialise(std::ofstream& file)
{
	utils::serialiseUint32(file, static_cast<uint32_t>(repeatable));
	utils::serialiseUint32(file, static_cast<uint32_t>(triggered));
}

void Interactable::deserialise(const char* buffer, size_t& offset)
{
	repeatable = static_cast<bool>(utils::deserialiseUint32(buffer, offset));
	triggered = static_cast<bool>(utils::deserialiseUint32(buffer, offset));
}

void Sprite::serialise(std::ofstream& file)
{
	utils::serialiseUint32(file, static_cast<uint32_t>(id));
	utils::serialiseUint32(file, static_cast<uint32_t>(clip_x));
	utils::serialiseUint32(file, static_cast<uint32_t>(clip_y));
	utils::serialiseUint32(file, static_cast<uint32_t>(width));
	utils::serialiseUint32(file, static_cast<uint32_t>(height));

	utils::serialiseUint8(file, r);
	utils::serialiseUint8(file, g);
	utils::serialiseUint8(file, b);
}

void Sprite::deserialise(const char* buffer, size_t& offset)
{
	id = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	clip_x = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	clip_y = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	width = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	height = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	r = utils::deserialiseUint8(buffer, offset);
	g = utils::deserialiseUint8(buffer, offset);
	b = utils::deserialiseUint8(buffer, offset);
}

void AnimFrame::serialise(std::ofstream& file)
{
	utils::serialiseUint32(file, static_cast<uint32_t>(id));
	utils::serialiseUint32(file, static_cast<uint32_t>(clip_x));
	utils::serialiseUint32(file, static_cast<uint32_t>(clip_y));
	utils::serialiseUint32(file, static_cast<uint32_t>(width));
	utils::serialiseUint32(file, static_cast<uint32_t>(height));
}

void AnimFrame::deserialise(const char* buffer, size_t& offset)
{
	id = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	clip_x = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	clip_y = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	width = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	height = static_cast<int>(utils::deserialiseUint32(buffer, offset));
}

void Animation::serialise(std::ofstream& file)
{
	utils::serialiseUint32(file, static_cast<uint32_t>(lifetime * 1000.0f));
	utils::serialiseUint32(file, static_cast<uint32_t>(dynamic));
	utils::serialiseUint32(file, static_cast<uint32_t>(_state));

	utils::serialiseUint32(file, static_cast<uint32_t>(animations.size()));
	for (auto it = animations.begin(); it != animations.end(); it++) {
		utils::serialiseUint32(file, static_cast<uint32_t>(it->first));
		utils::serialiseUint32(file, static_cast<uint32_t>(it->second.size()));
		std::for_each(it->second.begin(), it->second.end(), [&file](AnimFrame& anim) {anim.serialise(file); });
	}
}

void Animation::deserialise(const char* buffer, size_t& offset)
{
	lifetime = static_cast<float>(utils::deserialiseUint32(buffer, offset)) / 1000.0f;
	dynamic = static_cast<bool>(utils::deserialiseUint32(buffer, offset));
	_state = static_cast<state>(utils::deserialiseUint32(buffer, offset));

	auto num_states = utils::deserialiseUint32(buffer, offset);
	for (uint32_t i = 0; i < num_states; i++) {
		auto state_key = static_cast<state>(utils::deserialiseUint32(buffer, offset));
		auto num_frames = utils::deserialiseUint32(buffer, offset);
		std::deque<AnimFrame> frames;
		for (uint32_t j = 0; j < num_frames; j++) {
			AnimFrame a = AnimFrame();
			a.deserialise(buffer, offset);
			frames.push_back(a);
		}
		if (state_key == state::IDLE) {
			animations.at(state::IDLE) = frames;
		}
		else {
			animations.insert({ state_key, frames });
		}
	}
}

void Particle::serialise(std::ofstream& file)
{
	utils::serialiseUint32(file, static_cast<uint32_t>(lifetime));
}

void Particle::deserialise(const char* buffer, size_t& offset)
{
	lifetime = static_cast<float>(utils::deserialiseUint32(buffer, offset));
}

void Scriptable::serialise(std::ofstream& file)
{
	utils::serialiseUint32(file, owner);
	utils::serialiseString(file, scriptFile);
	utils::serialiseString(file, OnInit);
	utils::serialiseString(file, OnUpdate);
	utils::serialiseString(file, OnBump);
	utils::serialiseString(file, OnDeath);
	utils::serialiseString(file, OnUse);
	utils::serialiseString(file, OnEquip);
	utils::serialiseString(file, OnUnequip);
	utils::serialiseString(file, OnHit);
}

void Scriptable::deserialise(const char* buffer, size_t& offset)
{
	owner = utils::deserialiseUint32(buffer, offset);
	scriptFile = utils::deserialiseString(buffer, offset);
	OnInit = utils::deserialiseString(buffer, offset);
	OnUpdate = utils::deserialiseString(buffer, offset);
	OnBump = utils::deserialiseString(buffer, offset);
	OnDeath = utils::deserialiseString(buffer, offset);
	OnUse = utils::deserialiseString(buffer, offset);
	OnEquip = utils::deserialiseString(buffer, offset);
	OnUnequip = utils::deserialiseString(buffer, offset);
	OnHit = utils::deserialiseString(buffer, offset);
}

void Stat::serialise(std::ofstream& file)
{
	utils::serialiseUint32(file, static_cast<uint32_t>(base));
	utils::serialiseUint32(file, static_cast<uint32_t>(buff));
	utils::serialiseUint32(file, static_cast<uint32_t>(mod));
}

void Stat::deserialise(const char* buffer, size_t& offset)
{
	base = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	buff = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	mod = static_cast<int>(utils::deserialiseUint32(buffer, offset));
}

Fighter::Fighter(int _hp, int _defence, int _str, int _dex, int _con, int _wis, int _int, int _cha)
	: max_hp(_hp), hp(_hp), defence(_defence), 
	strength(_str, 0, utils::getAttributeMod(_str)),
	dexterity(_dex, 0, utils::getAttributeMod(_dex)),
	constitution(_con, 0, utils::getAttributeMod(_con)),
	wisdom(_wis, 0, utils::getAttributeMod(_wis)),
	intelligence(_int, 0, utils::getAttributeMod(_int)),
	charisma(_cha, 0, utils::getAttributeMod(_cha))
{
}

void Fighter::serialise(std::ofstream& file)
{
	utils::serialiseUint32(file, static_cast<uint32_t>(max_hp));
	utils::serialiseUint32(file, static_cast<uint32_t>(hp));
	utils::serialiseUint32(file, static_cast<uint32_t>(defence));
	utils::serialiseUint32(file, static_cast<uint32_t>(crit_mod));
	strength.serialise(file);
	dexterity.serialise(file);
	constitution.serialise(file);
	intelligence.serialise(file);
	wisdom.serialise(file);
	charisma.serialise(file);
}

void Fighter::deserialise(const char* buffer, size_t& offset)
{
	max_hp = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	hp = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	defence = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	crit_mod = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	strength.deserialise(buffer, offset);
	dexterity.deserialise(buffer, offset);
	constitution.deserialise(buffer, offset);
	intelligence.deserialise(buffer, offset);
	wisdom.deserialise(buffer, offset);
	charisma.deserialise(buffer, offset);
}

void Item::serialise(std::ofstream& file)
{
	utils::serialiseString(file, name);
	utils::serialiseString(file, description);
	utils::serialiseUint32(file, static_cast<uint32_t>(weight));
}

void Item::deserialise(const char* buffer, size_t& offset)
{
	name = utils::deserialiseString(buffer, offset);
	description = utils::deserialiseString(buffer, offset);
	weight = static_cast<int>(utils::deserialiseUint32(buffer, offset));
}

void Equipable::serialise(std::ofstream& file)
{
	utils::serialiseUint32(file, static_cast<uint32_t>(slot));
}

void Equipable::deserialise(const char* buffer, size_t& offset)
{
	slot = static_cast<Slot>(utils::deserialiseUint32(buffer, offset));
}

void Weapon::serialise(std::ofstream& file)
{
	utils::serialiseUint32(file, static_cast<uint32_t>(num_dice));
	utils::serialiseUint32(file, static_cast<uint32_t>(sides));
	utils::serialiseUint32(file, static_cast<uint32_t>(ranged));
	utils::serialiseString(file, ammo);
}

void Weapon::deserialise(const char* buffer, size_t& offset)
{
	num_dice = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	sides = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	ranged = static_cast<bool>(utils::deserialiseUint32(buffer, offset));
	ammo = utils::deserialiseString(buffer, offset);
}

void Armour::serialise(std::ofstream& file)
{
	utils::serialiseUint32(file, static_cast<uint32_t>(defence_bonus));
	utils::serialiseUint32(file, static_cast<uint32_t>(sides));
}

void Armour::deserialise(const char* buffer, size_t& offset)
{
	defence_bonus = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	sides = static_cast<int>(utils::deserialiseUint32(buffer, offset));
}

void AI::serialise(std::ofstream& file)
{
	utils::serialiseUint32(file, static_cast<uint32_t>(attitude));
	utils::serialiseUint32(file, static_cast<uint32_t>(blind));
	utils::serialiseUint32(file, static_cast<uint32_t>(scent));
	utils::serialiseUint32(file, xp);
}

void AI::deserialise(const char* buffer, size_t& offset)
{
	attitude = static_cast<Attitude>(utils::deserialiseUint32(buffer, offset));
	blind = static_cast<bool>(utils::deserialiseUint32(buffer, offset));
	scent = static_cast<bool>(utils::deserialiseUint32(buffer, offset));
	xp = utils::deserialiseUint32(buffer, offset);
}

void Container::serialise(std::ofstream& file)
{
	utils::serialiseUint32(file, static_cast<uint32_t>(weight));
	utils::serialiseUint32(file, static_cast<uint32_t>(weight_capacity));
	utils::serialiseVector(file, inventory);
}

void Container::deserialise(const char* buffer, size_t& offset)
{
	weight = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	weight_capacity = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	inventory = utils::deserialiseVector(buffer, offset);
}

void Stackable::serialise(std::ofstream& file)
{
	utils::serialiseUint32(file, static_cast<uint32_t>(quantity));
}

void Stackable::deserialise(const char* buffer, size_t& offset)
{
	quantity = static_cast<int>(utils::deserialiseUint32(buffer, offset));
}

void Body::serialise(std::ofstream& file)
{
	utils::serialiseVector(file, equipment);
}

void Body::deserialise(const char* buffer, size_t& offset)
{
	equipment = utils::deserialiseVector(buffer, offset);
}

void ID::serialise(std::ofstream& file)
{
	utils::serialiseString(file, id);
}

void ID::deserialise(const char* buffer, size_t& offset)
{
	id = utils::deserialiseString(buffer, offset);
}

void Useable::serialise(std::ofstream& file)
{
	utils::serialiseUint32(file, static_cast<uint32_t>(type));
	utils::serialiseUint32(file, static_cast<uint32_t>(charges));
	utils::serialiseUint32(file, static_cast<uint32_t>(base_charges));
}

void Useable::deserialise(const char* buffer, size_t& offset)
{
	type = static_cast<UseableType>(utils::deserialiseUint32(buffer, offset));
	charges = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	base_charges = static_cast<int>(utils::deserialiseUint32(buffer, offset));
}


