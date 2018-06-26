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
		EquipmentObject(sf::String, sf::String, sf::Texture*, equipmentObjectType);

		sf::Texture* getTexture();
		sf::String getName();
		sf::String getDescription();
		equipmentObjectType getType();

		bool usable = true;
	private:
		sf::Texture* _objectIcon;
		sf::String _objectDescription, _objectName, _objectId;

		equipmentObjectType _objectType;
	};
}