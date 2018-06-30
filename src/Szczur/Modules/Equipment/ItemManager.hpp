#pragma once
#include "WearableItem.hpp"
#include "Szczur/Modules/Script/Script.hpp"

namespace rat {
	class EquipmentObject;
	class ItemManager {
	public:
		ItemManager();
		std::map<std::string, EquipmentObject*> loadFromFile(Script& script);
		void setNewPath(std::string newPath);
	private:
		std::string _pathToFile;
		std::map<std::string, equipmentObjectType> _enumMap;
	};
}