#pragma once

#include "Common.hpp"

struct RandomNumber {
	RandomNumber() :seed(0), seed_generator(), random_generator() {
		seed = seed_generator();
		random_generator.seed(seed);
	};
	~RandomNumber() {};

	inline int sample(int min, int max) {
		return static_cast<int>((random_generator() % max) + min);
	};

	inline void new_seed() {
		seed = seed_generator();
		random_generator.seed(seed);
	}

	inline void set_seed(unsigned int _seed) {
		seed = _seed;
		random_generator.seed(seed);
	}

	unsigned int seed;
	std::random_device seed_generator;
	std::mt19937 random_generator;
};