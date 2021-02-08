#pragma once


struct CharacterClass {
	CharacterClass(std::string& _name, std::string& _desc, std::vector<int>& _stats, unsigned int _id, int _clip_x, int _clip_y, int _hit_die) :
		name(_name), description(_desc), stats(_stats), id(_id), clip_x(_clip_x), clip_y(_clip_y), hit_die(_hit_die)
	{

	};

	~CharacterClass() {};
	
	std::string name{ "" };
	std::string description{ "" };
	std::vector<int> stats;
	unsigned int id{ 0 };
	int clip_x{ 0 };
	int clip_y{ 0 };
	int hit_die{ 0 };
};