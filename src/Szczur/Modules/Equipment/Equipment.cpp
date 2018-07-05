#include "Equipment.hpp"
#include "Szczur/Modules/GUI/WindowWidget.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"

#include "NormalSlots.hpp"
#include "EquipmentSlot.hpp"
#include "ArmorSlots.hpp"
#include "ItemPreview.hpp"
#include "EquipmentObject.hpp"
#include "RingSlider.hpp"

#include "ItemManager.hpp"
#include "Szczur/Modules/GUI/InterfaceWidget.hpp"

#include "Szczur/Modules/GUI/Animation/Anim.hpp"

namespace rat {		//beware spagetti monster down there :/
	Equipment::Equipment()
	: _mainWindow(getModule<Window>()), _window(_mainWindow.getWindow())
	{
		LOG_INFO("Initializing Equipment module");
		_mainWindow.pushGLStates();
		_canvas.create(_window.getSize().x, _window.getSize().y);
		_mainWindow.popGLStates();
	
		initScript();
		init();		
		LOG_INFO("Module Equipment initialized");
		getModule<Script>().scriptFile("test.lua");
	}

	Equipment::~Equipment() {
		LOG_INFO("Module Input destructed");
	}

	void Equipment::initScript() {
		Script& script = getModule<Script>();
		auto module = script.newModule("Equipment");

		module.set("ITEM_TYPE_WEAPON", equipmentObjectType::weapon);
		module.set("ITEM_TYPE_ARMOR", equipmentObjectType::armor);
		module.set("ITEM_TYPE_AMULET", equipmentObjectType::amulet);
		module.set("ITEM_TYPE_RING", equipmentObjectType::ring);

		module.set_function("addUsableItem", &Equipment::addUsableItem, this);
		module.set_function("addWearableItem", &Equipment::addWearableItem, this);
		module.set_function("removeUsableItem", &Equipment::removeUsableItem, this);
		module.set_function("removeWearableItem", &Equipment::removeWearableItem, this);
		module.set_function("getUsableItem", &Equipment::getUsableItem, this);
		module.set_function("getWearableItem", &Equipment::getWearableItem, this);
		module.set_function("getItemTable", &Equipment::getItemsList, this);
		module.set_function("getFreeSlotsAmount", &Equipment::getFreeSlotsAmount, this);
		module.set_function("resizeSlots", &Equipment::resizeSlots, this);
		module.set_function("getSlotsAmount", &Equipment::getSlotsAmount, this);
		module.set_function("setSelectedRingsLimit", &Equipment::setSelectedRingsLimit, this);

		script.initClasses<WearableItem, UsableItem>();
	}

	void Equipment::init() {
		auto& gui = getModule<GUI>(); 

		_base = gui.addInterface();
		_base->setSizingWidthToHeightProportion(1.f);

		_equipmentFrame = new WindowWidget();
		_base->add(_equipmentFrame);
		_equipmentFrame->makeChildrenUnresizable();

		//_equipmentFrame->setPosition(sf::Vector2f(_window.getSize().x / 10 * 3, _window.getSize().y / 10 * 3.5f)); //base equipment widget
		//_equipmentFrame->setPosition(384.f, 220.f); //{384.f ,220.f }
		_equipmentFrame->setPropPosition(0.5f, 1.f);		
		_equipmentFrame->setPropSize(.63f, .7f);
		_equipmentFrame->setTexture(gui.getAsset<sf::Texture>("Assets/Test/NinePatchTest.png"), 200);
	
		_armorSlots = new ArmorSlots({0.11f, 0.11f}, this);
		_armorSlots->setParent(_equipmentFrame);
		_armorSlots->setPropPosition({0.0f, 0.0f});
		_armorSlots->initAssetsViaGUI(gui);

		_normalSlots = new NormalSlots(20, gui.getAsset<sf::Texture>("Assets/Equipment/slot.png"), gui.getAsset<sf::Texture>("Assets/Equipment/highlight.png"), { 62, 62 }, this);
		_normalSlots->setParent(_equipmentFrame);
		_normalSlots->setPropPosition(sf::Vector2f(0.f, 1.f));

		_ringSlider = new RingSlider({ 70u, 70u }, this);
		_ringSlider->setParent(_equipmentFrame);
		_ringSlider->initAssetsViaGUI(gui);
		_ringSlider->setPropPosition({ 1.f, 0.0f });

		_itemPreview = new ItemPreview(gui.getAsset<sf::Texture>("Assets/Equipment/slot.png"), gui.getAsset<sf::Texture>("Assets/Equipment/szczegoly.png"), {270, 90}, gui.getAsset<sf::Font>("Assets/Equipment/NotoMono.ttf"));
		_itemPreview->setParent(_base);
		_itemPreview->minimalize();

		_itemManager = new ItemManager;
		_itemManager->setNewPath("Assets/Equipment/items.json");
		_listOfObjects = _itemManager->loadFromFile(getModule<Script>());

		_equipmentPosition = _equipmentFrame->getPosition();
	}

	void Equipment::update(float deltaTime) {
		_normalSlots->update(deltaTime);
		if (_isPreviewOn)
			_itemPreview->setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(_window)));

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::I) && _isEquipmentHidden) {
			_equipmentFrame->setPropPosition({0.5f, 1.f}, { .2f, gui::Easing::EaseOutExpo , [this]() {_isEquipmentHidden = false; } });
			_equipmentFrame->fullyActivate();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::I) && !_isEquipmentHidden) {
			_equipmentFrame->setPropPosition({ 0.5f, 4.f }, { .2f, gui::Easing::EaseOutExpo , [this]() {_equipmentFrame->fullyDeactivate(); _isEquipmentHidden = true; } });				
		}
	}

	void Equipment::render() {
		_mainWindow.pushGLStates();

		_canvas.clear(sf::Color::Transparent);
		_canvas.display();

		_mainWindow.getWindow().draw(sf::Sprite(_canvas.getTexture()));

		_mainWindow.popGLStates();
	}

	void Equipment::enableItemPreview(EquipmentObject* item) {
		if (canPreviewBeInstantiated) {
			_itemPreview->setItem(item);
			_isPreviewOn = true;
		}
	}

	void Equipment::disableItemPreview() {
		_itemPreview->minimalize();
		_isPreviewOn = false;
	}

	UsableItem* Equipment::getUsableItem(std::string nameId) {
		UsableItem* temp = dynamic_cast<UsableItem*>(_listOfObjects.find(nameId)->second);
		if (temp) {
			return temp;
		}
		return nullptr;
	}

	WearableItem* Equipment::getWearableItem(std::string nameId) {
		WearableItem* temp = dynamic_cast<WearableItem*>(_listOfObjects.find(nameId)->second);
		if (temp) {
			return temp;
		}
		return nullptr;
	}

	void Equipment::addUsableItem(UsableItem* item) {
		_normalSlots->addItem(item);
	}

	void Equipment::addWearableItem(WearableItem* item) {
		switch (item->getType())
		{
		case equipmentObjectType::amulet:
			_armorSlots->addAmulet(item);
			break;
		case equipmentObjectType::armor:
			_armorSlots->setArmor(item);
			break;
		case equipmentObjectType::weapon:
			_armorSlots->setWeapon(item);
			break;
		case equipmentObjectType::ring:
			_ringSlider->addItem(item);
			break;
		}
	}

	bool Equipment::removeWearableItem(WearableItem* item) {
			switch (item->getType()) {
			case equipmentObjectType::armor:
				return false;		//you cannot remove either weapon or armor
			case equipmentObjectType::weapon:
				return false;
			case equipmentObjectType::amulet:
				return _armorSlots->removeAmulet(item->getNameId());
			case equipmentObjectType::ring:
				return false;
			}
		return false;
	}

	bool Equipment::removeUsableItem(UsableItem* item) {
		return _normalSlots->removeItem(item->getNameId());
	}

	sol::table Equipment::getItemsList() {
		sol::table _table = getModule<Script>().get().create_table();
		for (auto& it : _normalSlots->getItemMap())
		{
			_table.add(it.first);
		}
		for (auto& it : _armorSlots->getAmuletList())
		{
			if(it)
				_table.add(it->getNameId());
		}
		if (_armorSlots->getArmorSlot()->getItem()) {
			_table.add(_armorSlots->getArmorSlot()->getItem()->getNameId());
		}
		if (_armorSlots->getWeaponSlot()->getItem()) {
			_table.add(_armorSlots->getWeaponSlot()->getItem()->getNameId());
		}
		for (auto& it : _ringSlider->getRingsList())
		{
			_table.add(it->getNameId());
		}
		return _table;
	}

	int Equipment::getFreeSlotsAmount() {
		return _normalSlots->getFreeSlotsAmount();
	}

	void Equipment::resizeSlots(int newCapacity) {
		_normalSlots->resizeSlots(newCapacity);
	}

	int Equipment::getSlotsAmount() {
		return _normalSlots->getSlotsAmount();
	}
	
	void Equipment::setNewItemPath(std::string newPath){
		_itemManager->setNewPath(newPath);
	}
	void Equipment::reloadItemList() {
		_listOfObjects = _itemManager->loadFromFile(getModule<Script>());
	}

	void Equipment::setSelectedRingsLimit(int newSize) {
		_ringSlider->setSelectedRingsLimit(newSize);
	}
}