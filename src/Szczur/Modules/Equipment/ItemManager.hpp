#pragma once
#include "WearableItem.hpp"


namespace rat {
	class EquipmentObject;
	class ItemManager {
	public:
		std::map<std::string, EquipmentObject*> loadFromFile();
		void setNewPath(std::string newPath);
	private:
		std::string _pathToFile;
		std::map<std::string, equipmentObjectType> enumMap = boost::assign::map_list_of("weapon", equipmentObjectType::weapon)("armor", equipmentObjectType::armor)("ring", equipmentObjectType::ring)("amulet", equipmentObjectType::amulet);
	};
}