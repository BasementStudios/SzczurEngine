#pragma once

#include <string>
#include <vector>

#include "Szczur/Utility/Modules/Module.hpp"

#include "Szczur/Modules/Script/Script.hpp"

namespace rat {
	struct Skill {
	public:
		static void initScript(Script& script);

		const std::string& getNameID();
		void setNameID(const std::string& nameID);

		const std::string& getName();
		void setName(const std::string& name);

		const std::string& getDescription();
		void setDescription(const std::string& description);

		void setPPCost(int cost);
		int getPPcost();

		void setEssenceCost(size_t index, int value);
		int getEssenceCost(size_t index);

		void setSkillType(int type);
		int getSkillType();

		void setIsKnown(bool isKnown);
		bool getIsKnown();
	private:
		std::string nameID;

		std::string name;
		std::string description;

		int PPCost;

		int essenceContainer [4];

		int type;

		bool isKnown;
	};

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