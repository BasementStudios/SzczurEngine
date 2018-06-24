#include "EquipmentObject.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"

namespace rat {
	EquipmentObject::EquipmentObject(sf::String name, sf::String description, sf::Texture* icon, equipmentObjectType type)
		: _objectName(name), _objectDescription(description), _objectType(type), _objectIcon(icon)
	{

	}

	/*void EquipmentObject::setIcon(sf::Texture* icon) {
		_objectIcon = icon;	
	}

	void EquipmentObject::setDescription(sf::String description) {
		_objectDescription = description;
	}

	void EquipmentObject::setName(sf::String name) {
		_objectName = name;
	}

	void EquipmentObject::setType(equipmentObjectType type) {
		_objectType = type;
	}*/

	sf::Texture* EquipmentObject::getTexture() {
		return _objectIcon;
	}

	sf::String EquipmentObject::getName() {
		return _objectName;
	}

	sf::String EquipmentObject::getDescription() {
		return _objectDescription;
	}

	equipmentObjectType EquipmentObject::getType() {
		return _objectType;
	}
}