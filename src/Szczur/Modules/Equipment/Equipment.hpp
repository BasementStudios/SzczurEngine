#pragma once
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/Script/Script.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Utility/Logger.hpp"
#include "Szczur/Modules/GUI/Widget.hpp"

#include <vector>

#include "WearableItem.hpp"
#include "UsableItem.hpp"
namespace rat
{
	class WindowWidget; class NormalSlots; class ArmorSlots; class ImageWidget; class ItemPreview;
	class Equipment : public Module<Window, Input, GUI, Script> {
	public:

		void init();
		void initScript();
		void update(float deltaTime = (1.f / 60.f));
		void render();

		Equipment();

		void enableItemPreview(EquipmentObject* item);
		void disableItemPreview();

		bool canPreviewBeInstantiated;

		UsableItem* createUsableItem(std::string nameId);	//creating new item and adding it to list
		WearableItem* createWearableItem(std::string nameId);
		UsableItem* getUsableItem(std::string nameId);
		WearableItem* getWearableItem(std::string nameId);
		void addUsableItem(UsableItem* item);
		void addWearableItem(WearableItem* item);
		bool removeUsableItem(UsableItem* item);
		bool removeWearableItem(WearableItem* item);
		sol::table getItemsList();
		int getFreeSlotsAmount();
		void resizeSlots(int newCapacity);
		int getSlotsAmount();
		void setRingsLimit(int newCapacity);

		//cos z tym jsonem

	private:
		NormalSlots* _normalSlots;
		ArmorSlots* _armorSlots;
		ItemPreview* _itemPreview;

		Window& mainWindow;
		sf3d::RenderWindow& window;

		Widget* _base{ nullptr };
		WindowWidget* _equipmentFrame{ nullptr };

		sf::RenderTexture _canvas;

		bool isPreviewOn = false;

		std::map<std::string, EquipmentObject*> _listOfObjects;
	};
}