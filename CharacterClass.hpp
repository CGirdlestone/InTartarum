#pragma once


struct CharacterClass {
	CharacterClass(std::string& _name, std::string& _desc, std::vector<int>& _stats, std::vector<std::string>& _skills, unsigned int _id, int _clip_x, int _clip_y, int _hit_die) :
		name(_name), description(_desc), stats(_stats), skills(_skills), id(_id), clip_x(_clip_x), clip_y(_clip_y), hit_die(_hit_die)
	{

	};

	~CharacterClass() {};
	
	std::string name{ "" };
	std::string description{ "" };
	std::vector<int> stats;
	std::vector<std::string> skills;
	unsigned int id{ 0 };
	int clip_x{ 0 };
	int clip_y{ 0 };
	int hit_die{ 0 };
};