#include "UsableItem.hpp"

namespace rat {
	UsableItem::UsableItem(std::string nameId) 
		: EquipmentObject(nameId)
	{
	}

	void UsableItem::initScript(Script& script) {
		auto object = script.newClass<UsableItem>("usableItem", "Equipment");//, "../NewClass.lua");
		object.set("setName", &EquipmentObject::setName);
		object.set("setDescription", &EquipmentObject::setDescription);
		object.set("setIcon", &EquipmentObject::setIcon);
		object.set("setIsUsable", &UsableItem::setIsUseble);
		object.set("setCallback", &UsableItem::setCallback);
		object.init();
	}

	void UsableItem::setIsUseble(bool isUsable) {
		_isUsable = isUsable;
	}

	void UsableItem::setCallback(sol::function firstCallback)  {
		_callback = firstCallback;
	}

	bool UsableItem::useItem() {
		if (_callback.valid()) {
			if (_isUsable) {
				_callback();
				return true;
			}
			else {
				_callback();
				return false;
			}
		}
		return false;
	}
}