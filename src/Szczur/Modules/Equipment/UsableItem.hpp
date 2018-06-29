#pragma once
#include "EquipmentObject.hpp"
#include <Sol/sol.hpp>
#include "Szczur/Modules/Script/Script.hpp"

namespace rat {
	class UsableItem : public EquipmentObject
	{
	public:
		UsableItem(std::string nameId);

		void initScript(Script& script);

		void setIsUseble(bool isUsable);

		void setCallback(sol::function firstCallback);

		bool useItem();
	private:
		sol::function _callback;

		bool _isUsable = true;
	};
}