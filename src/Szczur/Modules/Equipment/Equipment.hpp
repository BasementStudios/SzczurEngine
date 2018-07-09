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
	enum statusOfEq {
		replaced, notReplaced, waiting
	};

	class WindowWidget; class NormalSlots; class ArmorSlots; class ImageWidget; class ItemPreview; 
	class RingSlider; class ItemManager; class InterfaceWidget; class ReplaceItem;
	class Equipment : public Module<Window, Input, GUI, Script> {
		friend class NormalSlots;
		friend class ReplaceItem;
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
	private:
		NormalSlots* _normalSlots;
		ArmorSlots* _armorSlots;
		ItemPreview* _itemPreview;
		ReplaceItem* _replaceItem;
		RingSlider* _ringSlider;
		ItemManager* _itemManager;

		Window& _mainWindow;
		sf3d::RenderWindow& _window;

		InterfaceWidget* _base{ nullptr };
		WindowWidget* _equipmentFrame{ nullptr };

		sf::RenderTexture _canvas;

		bool _isPreviewOn = false;
		bool _isEquipmentHidden = false;

		sf::Vector2f _equipmentPosition;

		std::map<std::string, EquipmentObject*> _listOfObjects;

		void _replaceNewItem(EquipmentObject* item);
		void _stopReplacingItem(bool hasBeenSuccesfull);
		statusOfEq _replacingStatus;

		statusOfEq lastChangeStatus();

		void _openEquipment();
		void _closeEquipment();
		bool _isEquipmentOpen();

		//lua
		bool _hasItem(const std::string&);
		bool _hasItem(const std::string&, int);
		UsableItem* getUsableItem(const std::string& nameId);
		WearableItem* getWearableItem(const std::string& nameId);
		bool addUsableItem(UsableItem* item);
		void addWearableItem(WearableItem* item);
		bool removeUsableItem(UsableItem* item);
		bool removeUsableItem(const std::string&);
		bool removeUsableItem(const std::string&, int);
		bool removeAllItems(const std::string&);
		bool removeWearableItem(WearableItem* item);
		sol::table getItemsList();
		int getFreeSlotsAmount();
		void setSelectedRingsLimit(int newSize);
		void resizeSlots(int newCapacity);
		int getSlotsAmount();
		bool hasChosenAmulet(const std::string&);
		bool hasArmor(const std::string&);
		bool hasWeapon(const std::string&);
		bool useItem(const std::string&);

	};
}