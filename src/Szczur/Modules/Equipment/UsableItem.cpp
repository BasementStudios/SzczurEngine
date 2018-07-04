#include "UsableItem.hpp"

namespace rat {
	UsableItem::UsableItem(std::string nameId) 
		: EquipmentObject(nameId)
	{
	}

	UsableItem::~UsableItem() {

	}

	void UsableItem::initScript(Script& script) {
		auto object = script.newClass<UsableItem>("UsableItem", "Equipment");//, "../NewClass.lua");
		object.set("setName", &EquipmentObject::setName);
		object.set("setDescription", &EquipmentObject::setDescription);
		object.set("setIcon", &EquipmentObject::setIcon);
		object.set("isUsable", &UsableItem::_isUsable);
		object.set("callback1", &UsableItem::_callback);
		object.init();
	}

	bool UsableItem::useItem() {
		if (_callback.valid() && _isUsable.valid() && _isUsable(this).get<bool>()) {
				_callback(this);
				return true;
		}
		return false;
	}
}