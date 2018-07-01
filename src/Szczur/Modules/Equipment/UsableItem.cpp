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
		object.set("setIsUsable", &UsableItem::setIsUseble);
		object.set("setCallback", &UsableItem::setCallback);
		object.set("callback1", &UsableItem::_callback);
		object.init();
	}

	void UsableItem::setIsUseble(bool isUsable) {
		_isUsable = isUsable;
	}

	void UsableItem::setCallback(std::string path)  {
		pathToScript = path;
	}

	bool UsableItem::useItem() {
		if (_callback.valid()) {
			if (_isUsable) {
				_callback(this);
				return true;
			}
			else {
				_callback(this);
				return false;
			}
		}
		return false;
	}
}