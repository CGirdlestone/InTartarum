#pragma once


struct CharacterClass {
	CharacterClass(std::string& _name, std::string& _desc, std::vector<int>& _stats, unsigned int _id, int _clip_x, int _clip_y) :
		name(_name), description(_desc), stats(_stats), id(_id), clip_x(_clip_x), clip_y(_clip_y)
	{

	};

	~CharacterClass() {};
	
	std::string name{ "" };
	std::string description{ "" };
	std::vector<int> stats;
	unsigned int id{ 0 };
	int clip_x{ 0 };
	int clip_y{ 0 };
};