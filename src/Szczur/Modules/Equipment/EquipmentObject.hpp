#pragma once
#include <SFML/Graphics.hpp>
#include "Szczur/Modules/GUI/Base/BaseBar.hpp"
/*
	class for items containing image widget of object, name and description
*/

namespace rat {
	enum equipmentObjectType {	//not used yet
		potion, amulet
	};

	class ImageWidget;

	class EquipmentObject : public BaseBar {
	public:
		EquipmentObject(sf::String, sf::String, sf::Texture*, equipmentObjectType);

		/*void setIcon(sf::Texture*);
		void setName(sf::String);
		void setDescription(sf::String);
		void setType(equipmentObjectType);*/

		sf::Texture* getIcon();
		sf::String getName();
		sf::String getDescription();
		equipmentObjectType getType();

	private:
		sf::Texture* _objectIcon;
		sf::String _objectDescription, _objectName;

		equipmentObjectType _objectType;
	};
}