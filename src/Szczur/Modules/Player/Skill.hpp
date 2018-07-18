#pragma once
#include <string>
#include "Szczur/Modules/Script/Script.hpp"
#include "SFML/Graphics.hpp"

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

		void setIcon(sf::Texture* _icon);
		sf::Texture* getIcon();
	private:
		std::string nameID;

		std::string name;
		std::string description;

		int PPCost;

		int essenceContainer[4];

		int type;

		bool isKnown;

		sf::Texture* icon;
	};
}