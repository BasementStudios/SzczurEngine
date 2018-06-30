#pragma once
#include "EquipmentObject.hpp"
#include <Sol/sol.hpp>
#include "Szczur/Modules/Script/Script.hpp"

namespace rat {
	class UsableItem : public EquipmentObject
	{
	public:
		UsableItem(std::string nameId);
		~UsableItem();

		static void initScript(Script& script);

		void setIsUseble(bool isUsable);

		void setCallback(std::string);

		bool useItem();
	private:
		std::string pathToScript;
		sol::function _callback;

		bool _isUsable = true;
	};
}