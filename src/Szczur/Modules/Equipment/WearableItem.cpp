#include "WearableItem.hpp"

namespace rat {
	WearableItem::WearableItem(std::string nameId) 
		: EquipmentObject(nameId)
	{
	}

	WearableItem::~WearableItem() {

	}

	void WearableItem::initScript(Script& script) {
		auto object = script.newClass<WearableItem>("WearableItem", "Equipment");
		object.set("setName", &EquipmentObject::setName);
		object.set("setDescription", &EquipmentObject::setDescription);
		object.set("setIcon", &EquipmentObject::setIcon);
		object.set("setIcon", &EquipmentObject::setIcon);
		object.set("onActivation", &WearableItem::_callback1);
		object.set("onDeactivation", &WearableItem::_callback2);
		object.init();
	}

	void WearableItem::activate() {
		if (_callback1.valid()) {
			_callback1(this);
		}
	}
	void WearableItem::deactivate() {
		if (_callback2.valid()) {
			_callback2(this);
		}
	}

	void WearableItem::setType(equipmentObjectType type) {
		_objectType = type;
	}
	equipmentObjectType WearableItem::getType() {
		return _objectType;
	}
}