#pragma once
#include "Szczur/Utility/Modules/Module.hpp"
#include "Szczur/Modules/Script/Script.hpp"
#include <vector>

namespace rat {
	struct Skill {
	public:
		static void initScript(Script& script);

		void setNameId(std::string& nameId);
		std::string& getNameId();

		void setName(std::string name);
		std::string& getName();

		void setDescription(std::string description);
		std::string& getDescription();

		void setPPCost(int cost);
		int getPPcost();

		void setEssenceCost(size_t index, int value);
		int getEssenceCost(size_t index);

		void setSkillType(int path);
		int getSkillType();
	private:
		std::string nameId;

		std::string name;
		std::string description;

		int PPCost;

		int essenceContainer [4];

		int type;
	};

	class Player : public Module<Script>
	{
	public:
		Player();
		~Player();

		void initJson();
		std::string& getPathToJson();
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

		bool addSkill(std::string& nameId);
		bool removeSkill(std::string& nameId);
		Skill* getSkill(std::string& nameId);
	private:
		std::string _pathToJson;

		int _currentHP;
		int _maxHP;

		int _currentPP;
		int _maxPP;

		int _essenceContainer [4];

		std::vector<Skill*> _skillsList;
		std::vector<Skill*> _knownSkillsList;
	};
}