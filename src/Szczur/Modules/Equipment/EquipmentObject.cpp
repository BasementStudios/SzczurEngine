#include "EquipmentObject.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include <Sol/sol.hpp>

namespace rat {
	EquipmentObject::EquipmentObject(std::string nameId)
		: _objectNameId(nameId)
	{
		_objectIcon.reset(new sf::Texture);
	}

	/*void EquipmentObject::setIcon(sf::Texture* text) {
		_objectIcon = text;
	}*/

	void EquipmentObject::setIcon(std::string path) {
		_objectIcon->loadFromFile(path);
	}

	void EquipmentObject::setName(std::string name) {
		_objectName = name;
	}
	void EquipmentObject::setDescription(std::string description) {
		_objectDescription = description;
	}

	sf::Texture* EquipmentObject::getTexture() {
		return _objectIcon.get();
	}

	std::string& EquipmentObject::getName() {
		return _objectName;
	}

	std::string& EquipmentObject::getDescription() {
		return _objectDescription;
	}

	std::string& EquipmentObject::getNameId() {
		return _objectNameId;
	}

	/*void EquipmentObject::setCallback(sol::function firstCallback) {
		callback1 = firstCallback;
		has2Callbacks = false;
	}

	void EquipmentObject::setCallback(sol::function firstCallback, sol::function secondCallback) {
		callback1 = firstCallback;
		callback2 = secondCallback;
		has2Callbacks = true;
	}*/

	/*bool EquipmentObject::useItem() {
		if (_objectType == equipmentObjectType::other && callback1.valid()) {
			if (usable) {
				callback1();
				return true;
			}
			else {
				callback1();
				return false;
			}
		}
		else {
			LOG_ERROR("trying to use non-item object or callback is invalid");
			return false;
		}
	}
	void EquipmentObject::activate() {
		if (_objectType != equipmentObjectType::other && callback1.valid()) {
			callback1();
		}
		else {
			LOG_ERROR("trying to activate item object or callback is invalid");
		}
	}
	void EquipmentObject::deactivate() {
		if (_objectType != equipmentObjectType::other && callback2.valid()) {
			callback2();
		}
		else {
			LOG_ERROR("trying to disactivate item object or callback is invalid");
		}
	}*/
}