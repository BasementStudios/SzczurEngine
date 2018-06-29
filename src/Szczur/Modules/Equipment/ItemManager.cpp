#include "ItemManager.hpp"
#include "UsableItem.hpp"

#include <nlohmann/json.hpp>
#include <fstream>

#include <boost/assign.hpp>

namespace rat {
	std::map<std::string, EquipmentObject*> ItemManager::loadFromFile()
	{
		std::map<std::string, EquipmentObject*> temp;

		nlohmann::json config;

		std::ifstream file(_pathToFile);

		if (file.good()) {
			file >> config;

			if (auto& items = config["items"]; !items.is_null()) {

				for (auto& item : items) {
					//wearable item
					if (!item["type"].is_null() && item["type"].get<std::string>() == "wearable") {
						WearableItem* newItem = new WearableItem(item.get<std::string>());
						if (!item["icon"].is_null()) { newItem->setIcon(item["icon"].get<std::string>()); }
						else { assert(false); }	//item file is invalid
						if (!item["script"].is_null()) { newItem->setCallback(item["script"].get<std::string>()); }
						else { assert(false);}
						if (!item["category"].is_null()) {
							if (enumMap.find(item["category"].get<std::string>()) != enumMap.end()) { newItem->setType(enumMap.find(item["category"].get<std::string>())->second); }
							else { assert(false); }
						}
						if (!item["pl"]["name"].is_null()) { newItem->setName(item["pl"]["name"].get<std::string>()); }
						else { assert(false); }
						if (!item["pl"]["description"].is_null()) { newItem->setDescription(item["pl"]["description"].get<std::string>()); }
						else { assert(false); }
						temp.insert(std::make_pair(item.get<std::string>(), newItem));
					}
					//usable item
					if (!item["type"].is_null() && item["type"].get<std::string>() == "usable") {
						UsableItem* newItem = new UsableItem(item.get<std::string>());
						if (!item["icon"].is_null()) { newItem->setIcon(item["icon"].get<std::string>()); }
						else { assert(false); }	//item file is invalid
						if (!item["script"].is_null()) { newItem->setCallback(item["script"].get<std::string>()); }
						else { assert(false); }
						if (!item["pl"]["name"].is_null()) { newItem->setName(item["pl"]["name"].get<std::string>()); }
						else { assert(false); }
						if (!item["pl"]["description"].is_null()) { newItem->setDescription(item["pl"]["description"].get<std::string>()); }
						else { assert(false); }
						temp.insert(std::make_pair(item.get<std::string>(), newItem));
					}
				}

			}
			return temp;

		}

	}
	void ItemManager::setNewPath(std::string newPath)
	{
		_pathToFile = newPath;
	}
}