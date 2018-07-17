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
	enum StatusOfEq {
		replaced, notReplaced, waiting
	};

	class WindowWidget; class NormalSlots; class ArmorSlots; class ImageWidget; class ItemPreview; 
	class RingSlider; class ItemManager; class InterfaceWidget; class ReplaceItem; class Necklace;
	class EquipmentSlot;
	class Equipment : public Module<Window, Input, GUI, Script> {
		friend class NormalSlots;
		friend class ReplaceItem;
		friend class Necklace;
		friend class RingSlider;
	public:

		void init();
		void initScript();
		void update(float deltaTime = (1.f / 60.f));

		Equipment();
		~Equipment();

		void enableItemPreview(EquipmentObject* item, sf::Vector2f pos);
		void disableItemPreview();

		bool canPreviewBeInstantiated;

		void setNewItemsPath(std::string);
		void reloadItemsList();
		std::string& getPathToJson();

		void startEquipment();
		void stopEquipment();
	private:
		std::string _pathToJson;

		NormalSlots* _normalSlots = nullptr;
		ArmorSlots* _armorSlots = nullptr;
		ItemPreview* _itemPreview = nullptr;
		ReplaceItem* _replaceItem = nullptr;
		RingSlider* _ringSlider = nullptr;
		ItemManager* _itemManager = nullptr;
		Necklace* _necklace = nullptr;

		Window& _mainWindow;
		sf3d::RenderWindow& _window;
		Input& _input;

		InterfaceWidget* _base = nullptr;
		ImageWidget* _armorImage = nullptr;
		ImageWidget* _weaponImage = nullptr;
		ImageWidget* _background = nullptr;
		ImageWidget* _equipmentFrame = nullptr;

		bool _isPreviewOn = false;
		bool _isPreviewMaximized = false;
		float _timeFromStartingPreview;

		bool _isEquipmentHidden = true;

		sf::Vector2f _equipmentPosition;

		std::map<std::string, EquipmentObject*> _listOfObjects;

		bool _stoneStatusChanged(WearableItem* stone, bool status);

		void _replaceNewItem(EquipmentObject* item);
		void _stopReplacingItem(bool hasBeenSuccesfull);
		StatusOfEq _replacingStatus;
		bool _isReplacing;

		StatusOfEq lastChangeStatus();

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
		sol::table getItemsList();
		int getFreeSlotsAmount();
		void resizeSlots(int newCapacity);
		int getSlotsAmount();
		bool hasChosenStone(std::string& nameId);
		/*bool hasChosenAmulet(const std::string&);
		bool hasArmor(const std::string&);
		bool hasWeapon(const std::string&);*/
		bool useItem(const std::string&);

	};
}