#pragma once

#include <string>
#include <vector>

#include "Skill.hpp"

#include "Szczur/Utility/Modules/Module.hpp"
#include "Szczur/Modules/Script/Script.hpp"

namespace rat {
	class Player : public Module<Script>
	{
	public:
		Player();
		~Player();

		void initJson();
		const std::string& getPathToJson();
		void reloadJson();

		void initScript();

		int getHP();
		void addHP(int HP);
		void setHP(int newHP);

		int getMaxHP();
		void setMaxHP(int newMaxHP);

		int getPP();
		void addPP(int PP);
		void setPP(int PP);

		int getMaxPP();
		void setMaxPP(int newMaxPP);

		bool addSkill(const std::string& nameID);
		bool removeSkill(const std::string& nameID);
		Skill getSkill(const std::string& nameID); // @todo ? const? nie?

	private:
		std::string _pathToJson;

		int _currentHP;
		int _maxHP;

		int _currentPP;
		int _maxPP;

		int _essenceContainer [4];

		std::vector<Skill> _skillsList;
	};
}