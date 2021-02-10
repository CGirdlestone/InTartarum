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
