#pragma once
#include <SFML/Graphics.hpp>
#include "Szczur/Modules/GUI/Base/BaseBar.hpp"
/*
	class for items containing image widget of object, name and description
*/

namespace rat {
	enum equipmentObjectType {	//not used yet
		potion, amulet, armor, sword, ring, other
	};

	class ImageWidget;

	class EquipmentObject : public BaseBar {
	public:
		EquipmentObject(std::string, std::string, sf::Texture*, equipmentObjectType);

		sf::Texture* getTexture();
		std::string getName();
		std::string getDescription();
		equipmentObjectType getType();

		bool usable = true;
	private:
		sf::Texture* _objectIcon;
		std::string _objectDescription, _objectName, _objectId;

		equipmentObjectType _objectType;
	};
}