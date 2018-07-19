#pragma once
#include "EquipmentObject.hpp"
#include <sol.hpp>
#include "Szczur/Modules/Script/Script.hpp"

namespace rat {
	class UsableItem : public EquipmentObject
	{
	public:
		UsableItem(std::string nameId);
		~UsableItem();

		static void initScript(Script& script);

		bool useItem();
	private:
		std::string pathToScript;

		sol::function _callback;

		bool _destroyed;
		void _destroy();
	};
}