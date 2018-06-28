#include "UsableItem.hpp"

namespace rat {
	UsableItem::UsableItem(std::string nameId) 
		: EquipmentObject(nameId)
	{
	}
	void UsableItem::setIsUseble(bool isUsable) {
		_isUsable = isUsable;
	}

	void UsableItem::setCallback(sol::function firstCallback) {
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