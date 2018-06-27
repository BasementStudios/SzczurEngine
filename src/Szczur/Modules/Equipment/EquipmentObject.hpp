#pragma once
#include <SFML/Graphics.hpp>
#include "Szczur/Modules/GUI/Base/BaseBar.hpp"
/*
	class for items containing image widget of object, name and description
*/

namespace rat {
	enum equipmentObjectType {	//not used yet
		amulet, armor, weapon, ring, other
	};

	class ImageWidget;

	class EquipmentObject : public BaseBar {
	public:
		EquipmentObject(std::string nameId, std::string name, std::string description, sf::Texture* itemTex, equipmentObjectType type);

		sf::Texture* getTexture();
		std::string getName();
		std::string getDescription();
		std::string getNameId();
		equipmentObjectType getType();

		bool usable = true;
	private:
		sol::function* callback1;
		sol::function* callback2;

		sf::Texture* _objectIcon;
		std::string _objectDescription, _objectName, _objectId;

		equipmentObjectType _objectType;
	};
}