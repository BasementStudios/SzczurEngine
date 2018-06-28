#pragma once
#include "EquipmentObject.hpp"
namespace rat {
	class UsableItem : public EquipmentObject
	{
	public:
		UsableItem(std::string nameId);

		void setIsUseble(bool isUsable);

		void setCallback(sol::function firstCallback);

		bool useItem();
	private:
		sol::function _callback;

		bool _isUsable = true;
	};
}