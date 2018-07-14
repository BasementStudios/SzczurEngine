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
		object.set("callback", &UsableItem::_callback);
		object.set("destroy", &UsableItem::_destroy);
		object.init();
	}

	void UsableItem::_destroy() {
		_destroyed = true;
	}

	bool UsableItem::useItem() {
		if (_callback.valid()) {
			_callback(this);
			if(_destroyed)
				return true;
		}
		return false;
	}
}