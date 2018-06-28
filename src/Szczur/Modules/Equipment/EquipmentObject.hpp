#pragma once
#include <SFML/Graphics.hpp>
#include "Szczur/Modules/GUI/Base/BaseBar.hpp"
#include <Sol/sol.hpp>
/*
	class for items containing image widget of object, name and description
*/

namespace rat {

	class ImageWidget;

	class EquipmentObject : public BaseBar {
	public:
		EquipmentObject(std::string nameId);

		sf::Texture* getTexture();
		std::string getName();
		std::string getDescription();
		std::string getNameId();
		
		//cos z tekstura
		void setName(std::string name);
		void setDescription(std::string description);

		void setCallback(sol::function firstCallback);
		void setCallback(sol::function firstCallback, sol::function secondCallback);

		/*bool useItem();
		void activate();
		void deactivate();*/
	private:
		sol::function callback1;
		sol::function callback2;

		bool has2Callbacks;
		bool usable;

		sf::Texture* _objectIcon;
		std::string _objectDescription, _objectName, _objectNameId;

	};
}