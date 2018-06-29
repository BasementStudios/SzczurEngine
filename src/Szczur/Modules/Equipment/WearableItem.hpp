#pragma once
#include "EquipmentObject.hpp"
#include <sol.hpp>
#include "Szczur/Modules/Script/Script.hpp"

namespace rat {
	enum equipmentObjectType {
		amulet, armor, weapon, ring
	};

	class WearableItem : public EquipmentObject
	{
	public:
		WearableItem(std::string nameId);

		void initScript(Script& script);

		void setCallback(sol::function firstCallback, sol::function secondCallback);

		void activate();
		void deactivate();

		void setType(equipmentObjectType type);
		equipmentObjectType getType();
	private:
		sol::function _callback1;
		sol::function _callback2;

		equipmentObjectType _objectType;
	};
}