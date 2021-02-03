#pragma once


struct CharacterClass {
	CharacterClass(std::string& _name, std::string& _desc, std::vector<int>& _stats) :
		name(_name), description(_desc), stats(_stats)
	{
	};

	~CharacterClass() {};
	
	std::string name{ "" };
	std::string description{ "" };
	std::vector<int> stats;
};