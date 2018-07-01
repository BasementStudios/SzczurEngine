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
	class WindowWidget; class NormalSlots; class ArmorSlots; class ImageWidget; class ItemPreview; class RingSlider; class ItemManager;
	class Equipment : public Module<Window, Input, GUI, Script> {
	public:

		void init();
		void initScript();
		void update(float deltaTime = (1.f / 60.f));
		void render();

		Equipment();
		~Equipment();

		void enableItemPreview(EquipmentObject* item);
		void disableItemPreview();

		bool canPreviewBeInstantiated;

		void setNewItemPath(std::string);
		void reloadItemList();

		//UsableItem* createUsableItem(std::string nameId);	//creating new item and adding it to list
		//WearableItem* createWearableItem(std::string nameId);
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
	private:
		NormalSlots* _normalSlots;
		ArmorSlots* _armorSlots;
		ItemPreview* _itemPreview;
		RingSlider* _ringSlider;
		ItemManager* _itemManager;

		Window& _mainWindow;
		sf3d::RenderWindow& _window;

		Widget* _base{ nullptr };
		WindowWidget* _equipmentFrame{ nullptr };

		sf::RenderTexture _canvas;

		bool _isPreviewOn = false;
		bool _isEquipmentHidden = false;

		sf::Vector2f _equipmentPosition;

		std::map<std::string, EquipmentObject*> _listOfObjects;
	};
}