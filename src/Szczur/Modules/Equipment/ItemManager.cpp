#include "ItemManager.hpp"
#include "UsableItem.hpp"

#include <nlohmann/json.hpp>
#include <fstream>

namespace rat {

	ItemManager::ItemManager() {
		_enumMap.insert(std::make_pair("weapon", equipmentObjectType::weapon));
		_enumMap.insert(std::make_pair("armor", equipmentObjectType::armor));
		_enumMap.insert(std::make_pair("stone", equipmentObjectType::stone));
		_enumMap.insert(std::make_pair("amulet", equipmentObjectType::amulet));
	}
	std::map<std::string, EquipmentObject*> ItemManager::loadFromFile(Script& script)
	{
		std::map<std::string, EquipmentObject*> temp;

		nlohmann::json config;

		std::ifstream file(_pathToFile);

		if (file.good()) {
			file >> config;
			if (auto& items = config["items"]; !items.is_null()) {
				try {
					for (auto itr = config["items"].begin(); itr != config["items"].end(); ++itr) {
						//wearable item
						auto& item = itr.value();

						if (auto& type = item["type"]; !item["type"].is_null()) {
							std::string typeData = type;

							// Create new item
							EquipmentObject* newItem = nullptr;
							if (typeData == "wearable") newItem = new WearableItem(itr.key());
							else if (typeData == "usable") newItem = new UsableItem(itr.key());

							// Load category
							if (typeData == "wearable") {
								if (auto& var = item["category"]; !var.is_null()) {
									if (auto result = _enumMap.find(var); result != _enumMap.end()) {
										static_cast<WearableItem*>(newItem)->setType(result->second);
									}
									else { assert(false); }
								}
							}

							// Load icon
							if (auto& var = item["icon"]; !var.is_null()) {
								newItem->setIcon(var);
							}
							else { assert(false); }	//item file is invalid

							// Load name and description
							std::string language = "pl";
							if (auto& var = item[language]; !var.is_null()) {
								// Name
								if (auto& name = var["name"]; !name.is_null()) {
									newItem->setName(name);
								}
								else { assert(false); }

								// Description
								if (auto& description = var["description"]; !description.is_null()) {
									newItem->setDescription(description);
								}
								else { assert(false); }
							}

							// Load script
							if (auto& var = item["script"]; !var.is_null()) {
								if (typeData == "wearable") {
									script.get()["THIS"] = static_cast<WearableItem*>(newItem);
								}
								else if (typeData == "usable") {
									script.get()["THIS"] = static_cast<UsableItem*>(newItem);
								}
								script.scriptFile(var);
								script.get()["THIS"] = sol::nil;
							}
							else { assert(false); }

							temp.insert(std::make_pair(itr.key(), newItem));
						}
					}
				}
				catch (nlohmann::json::exception e) {
					LOG_EXCEPTION(e);
				}
				return temp;
			}
		}
	}
	void ItemManager::setNewPath(std::string newPath)
	{
		_pathToFile = newPath;
	}
}