#pragma once
#include <vector>
#include "SFML/Graphics.hpp"

namespace rat {
	class Widget; class ImageWidget; class Skill; class ListWidget; class GUI; class TextAreaWidget;
	class SkillSlots
	{
	public:
		SkillSlots(GUI& gui);

		void initGUI(GUI& gui);
		void setParent(Widget* base);
		void setPropPosition(sf::Vector2f);

		void addSkill(Skill* skill);

		void chooseSkill(const std::string& nameID);
		void unChooseSkill(const std::string& nameID);

		void setCounter(const std::string& nameID, const std::string& number);

		void clear();
	private:
		struct SkillSlot {
			TextAreaWidget* itemCounter = nullptr;
			ImageWidget* slotImage = nullptr;
			ImageWidget* skillImage = nullptr;
			ImageWidget* PPImage = nullptr;
			Skill* skill = nullptr;
			bool isChosen;
		};
		Widget* _base = nullptr;

		bool _isSomeSlotChosen;

		sf::Font* _font = nullptr;
		sf::Texture* _slotTexture;
		sf::Texture* _chosenSlotTexture;
		std::vector<sf::Texture*> _PPList;

		ListWidget* _slotsListWidget;
		std::vector<SkillSlot*> _slotsList;	
	};
}