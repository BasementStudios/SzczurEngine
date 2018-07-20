#pragma once

#include <string>
#include <vector>

#include "Skill.hpp"

#include "Szczur/Utility/Modules/Module.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/Script/Script.hpp"

namespace rat {
	class InterfaceWidget; class HPBar; class ImageWidget; class TimeBar; class PPCount; class SkillSlots; class ItemSlots;
	class Player : public Module<Script, GUI>
	{
	public:
		Player();
		~Player();

		void initJson();
		void initGUI();

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
		const Skill& getSkill(const std::string& nameID); // @todo ? const? nie?

		void setHPBarIndex(int);

		void setTimeBarIndex(int);
		void setCurrentTime(int);
		void setMaxTime(int);

		/*void setBrokenPPAmount(int);
		void setGoodPPAmount(int);*/
		void setPPSlotsAmount(int good, int broken);
		void setHighlightedPPAmount(int newAmount);

		void addSkillToSlot(const std::string&);
		void chooseSkill(const std::string&);
		void unChooseSkill(const std::string&);
		void setCounter(const std::string&, const std::string& number);
		void setPPCost(const std::string&, int number);

		void addItem(const std::string&, sf::Texture*);
		void removeItem(const std::string&);

		void start();
		void stop();

		void clearSkillSlots();

		void setHPbarStatus(const std::string&);
		void removeHPbarStatus(const std::string&);
	private:
		InterfaceWidget* _base = nullptr;
		ImageWidget* _background = nullptr;

		ImageWidget* _HPBack;
		HPBar* _HPBar;
		TimeBar* _TimeBar;

		ImageWidget* _PPBack;
		PPCount* _PPCount;
		SkillSlots* _skillSlots;

		ImageWidget* _itemBack;
		ItemSlots* _itemSlots;

		std::string _pathToJson;

		int _currentHP;
		int _maxHP;

		int _currentPP;
		int _maxPP;

		int _essenceContainer [4];

		std::vector<Skill> _skillsList;
	};
}