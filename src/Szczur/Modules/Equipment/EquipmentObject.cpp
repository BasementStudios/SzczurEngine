#include "EquipmentObject.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"

namespace rat {
	EquipmentObject::EquipmentObject(std::string name, std::string description, sf::Texture* icon, equipmentObjectType type)
		: _objectName(name), _objectDescription(description), _objectType(type), _objectIcon(icon)
	{

	}

	sf::Texture* EquipmentObject::getTexture() {
		return _objectIcon;
	}

	std::string EquipmentObject::getName() {
		return _objectName;
	}

	std::string EquipmentObject::getDescription() {
		return _objectDescription;
	}

	equipmentObjectType EquipmentObject::getType() {
		return _objectType;
	}
}