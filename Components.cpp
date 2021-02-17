#include "Components.hpp"

void Position::serialise(std::ofstream& file)
{
	utils::serialiseUint32(file, static_cast<uint32_t>(x));
	utils::serialiseUint32(file, static_cast<uint32_t>(y));
	utils::serialiseUint32(file, static_cast<uint32_t>(z));
}

void Position::deserialise(const char* buffer, size_t& offset)
{
	x = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	y = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	z = static_cast<int>(utils::deserialiseUint32(buffer, offset));
}

void Player::serialise(std::ofstream& file)
{
	utils::serialiseUint32(file, static_cast<uint32_t>(level));
	utils::serialiseUint32(file, static_cast<uint32_t>(vision));
}

void Player::deserialise(const char* buffer, size_t& offset)
{
	level = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	vision = static_cast<int>(utils::deserialiseUint32(buffer, offset));
}

void Actor::serialise(std::ofstream& file)
{
	// nothing yet
}

void Actor::deserialise(const char* buffer, size_t& offset)
{
	// nothing yet
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
	utils::serialiseString(file, OnInit);
	utils::serialiseString(file, OnUpdate);
	utils::serialiseString(file, OnBump);
	utils::serialiseString(file, OnDeath);
}

void Scriptable::deserialise(const char* buffer, size_t& offset)
{
	owner = utils::deserialiseUint32(buffer, offset);
	OnInit = utils::deserialiseString(buffer, offset);
	OnUpdate = utils::deserialiseString(buffer, offset);
	OnBump = utils::deserialiseString(buffer, offset);
	OnDeath = utils::deserialiseString(buffer, offset);
}

Fighter::Fighter(int _hp, int _defence, int _str, int _dex, int _con, int _wis, int _int, int _cha)
	: max_hp(_hp), hp(_hp), defence(_defence),
	base_strength(_str), str_buff(0), str_mod(utils::getAttributeMod(_str)),
	base_dexterity(_str), dex_buff(0), dex_mod(utils::getAttributeMod(_dex)),
	base_constitution(_str), con_buff(0), con_mod(utils::getAttributeMod(_con)),
	base_wisdom(_str), wis_buff(0), wis_mod(utils::getAttributeMod(_wis)),
	base_intelligence(_str), int_buff(0), int_mod(utils::getAttributeMod(_int)),
	base_charisma(_str), cha_buff(0), cha_mod(utils::getAttributeMod(_cha))
{
}

void Fighter::serialise(std::ofstream& file)
{
	utils::serialiseUint32(file, static_cast<uint32_t>(max_hp));
	utils::serialiseUint32(file, static_cast<uint32_t>(hp));
	utils::serialiseUint32(file, static_cast<uint32_t>(defence));
	utils::serialiseUint32(file, static_cast<uint32_t>(base_strength));
	utils::serialiseUint32(file, static_cast<uint32_t>(str_buff));
	utils::serialiseUint32(file, static_cast<uint32_t>(str_mod));
	utils::serialiseUint32(file, static_cast<uint32_t>(base_dexterity));
	utils::serialiseUint32(file, static_cast<uint32_t>(dex_buff));
	utils::serialiseUint32(file, static_cast<uint32_t>(dex_mod));
	utils::serialiseUint32(file, static_cast<uint32_t>(base_constitution));
	utils::serialiseUint32(file, static_cast<uint32_t>(con_buff));
	utils::serialiseUint32(file, static_cast<uint32_t>(con_mod));
	utils::serialiseUint32(file, static_cast<uint32_t>(base_wisdom));
	utils::serialiseUint32(file, static_cast<uint32_t>(wis_buff));
	utils::serialiseUint32(file, static_cast<uint32_t>(wis_mod));
	utils::serialiseUint32(file, static_cast<uint32_t>(base_intelligence));
	utils::serialiseUint32(file, static_cast<uint32_t>(int_buff));
	utils::serialiseUint32(file, static_cast<uint32_t>(int_mod));
	utils::serialiseUint32(file, static_cast<uint32_t>(base_charisma));
	utils::serialiseUint32(file, static_cast<uint32_t>(cha_buff));
	utils::serialiseUint32(file, static_cast<uint32_t>(cha_mod));
}

void Fighter::deserialise(const char* buffer, size_t& offset)
{
	max_hp = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	hp = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	defence = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	base_strength = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	str_buff = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	str_mod = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	base_dexterity = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	dex_buff = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	dex_mod = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	base_constitution = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	con_buff = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	con_mod = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	base_wisdom = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	wis_buff = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	wis_mod = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	base_intelligence = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	int_buff = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	int_mod = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	base_charisma = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	cha_buff = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	cha_mod = static_cast<int>(utils::deserialiseUint32(buffer, offset));
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
}

void Weapon::deserialise(const char* buffer, size_t& offset)
{
	num_dice = static_cast<int>(utils::deserialiseUint32(buffer, offset));
	sides = static_cast<int>(utils::deserialiseUint32(buffer, offset));
}

void AI::serialise(std::ofstream& file)
{
	utils::serialiseUint32(file, static_cast<uint32_t>(attitude));
}

void AI::deserialise(const char* buffer, size_t& offset)
{
	attitude = static_cast<Attitude>(utils::deserialiseUint32(buffer, offset));
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
