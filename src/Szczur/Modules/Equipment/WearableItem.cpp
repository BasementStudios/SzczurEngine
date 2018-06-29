#include "WearableItem.hpp"

namespace rat {
	WearableItem::WearableItem(std::string nameId) 
		: EquipmentObject(nameId)
	{
	}

	void WearableItem::initScript(Script& script) {
		auto object = script.newClass<WearableItem>("wearableItem", "Equipment");
		object.set("setName", &EquipmentObject::setName);
		object.set("setDescription", &EquipmentObject::setDescription);
		object.set("setIcon", &EquipmentObject::setIcon);
		object.set("setCallback", &WearableItem::setCallback);
		object.set("setType", &WearableItem::setType);
		object.set("setIcon", &EquipmentObject::setIcon);
		object.init();
	}

	void WearableItem::setCallback(sol::function firstCallback, sol::function secondCallback) {
		_callback1 = firstCallback;
		_callback2 = secondCallback;
	}

	void WearableItem::activate() {
		if (_callback1.valid()) {
			_callback1();
		}
	}
	void WearableItem::deactivate() {
		if (_callback2.valid()) {
			_callback2();
		}
	}

	void WearableItem::setType(equipmentObjectType type) {
		_objectType = type;
	}
	equipmentObjectType WearableItem::getType() {
		return _objectType;
	}
}