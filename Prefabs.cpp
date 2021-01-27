#include "Prefabs.hpp"

void Prefab::create_explosion(World& world, float x, float y, unsigned int tex)
{
	float dt = 0.05f;
	auto exp = world.CreateEntity();
	world.AddComponent<Position>(exp, x, y, 0);
	world.AddComponent<Sprite>(exp, tex, 0, 0, 64, 64, 0);
	world.AddComponent<Animation>(exp, dt, tex, 0, 0, 64, 64);
	auto exp_animation = world.GetComponent<Animation>(exp);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			exp_animation->animations.at(state::IDLE).push_back(AnimFrame(tex, 64 * j, 64 * i, 64, 64));
		}
	}
	world.AddComponent<Particle>(exp, static_cast<float>(exp_animation->animations.at(state::IDLE).size()) * dt);
}

