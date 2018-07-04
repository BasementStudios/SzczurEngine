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
		~WearableItem();

		static void initScript(Script& script);

		void activate();
		void deactivate();

		void setType(equipmentObjectType type);
		equipmentObjectType getType();

		sol::function _callback1;
		sol::function _callback2;
	private:		
		equipmentObjectType _objectType;
	};
}