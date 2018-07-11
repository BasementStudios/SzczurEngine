#include "Equipment.hpp"
#include "Szczur/Modules/GUI/WindowWidget.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"

#include "NormalSlots.hpp"
#include "EquipmentSlot.hpp"
#include "ArmorSlots.hpp"
#include "ItemPreview.hpp"
#include "EquipmentObject.hpp"
#include "RingSlider.hpp"
#include "ReplaceItem.hpp"

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

		module.set("NOT_REPLACED", statusOfEq::notReplaced);
		module.set("REPLACED", statusOfEq::replaced);
		module.set("WAITING", statusOfEq::waiting);

		module.set_function("addUsableItem", &Equipment::addUsableItem, this);
		module.set_function("addWearableItem", &Equipment::addWearableItem, this);
		//module.set_function("removeWearableItem", &Equipment::removeWearableItem, this);
		module.set_function("getUsableItem", &Equipment::getUsableItem, this);
		module.set_function("getWearableItem", &Equipment::getWearableItem, this);
		module.set_function("getItemTable", &Equipment::getItemsList, this);
		module.set_function("getFreeSlotsAmount", &Equipment::getFreeSlotsAmount, this);
		module.set_function("resizeSlots", &Equipment::resizeSlots, this);
		module.set_function("getSlotsAmount", &Equipment::getSlotsAmount, this);
		module.set_function("setSelectedRingsLimit", &Equipment::setSelectedRingsLimit, this);
		module.set_function("getLastChangeStatus", &Equipment::lastChangeStatus, this);
		module.set_function("closeEquipment", &Equipment::_closeEquipment, this);
		module.set_function("openEquipment", &Equipment::_openEquipment, this);
		module.set_function("isEquipmentOpen", &Equipment::_isEquipmentOpen, this);

		module.set_function("removeItem", sol::overload(
			[&](const std::string& a) {return removeUsableItem(a); },
			[&](const std::string& a, int b) {return removeUsableItem(a, b); },
			[&](UsableItem* a) {return removeUsableItem(a); }
		));
		module.set_function("removeAllItems", [&](const std::string& a) {return removeUsableItem(a, 0); });

		module.set_function("hasItem", sol::overload(
			[&](const std::string& a) {return _hasItem(a); },
			[&](const std::string& a, int b) {return _hasItem(a, b); }
		));
		module.set_function("hasChosenAmulet", &Equipment::hasChosenAmulet, this);
		module.set_function("hasArmor", &Equipment::hasArmor, this);
		module.set_function("hasWeapon", &Equipment::hasWeapon, this);
		module.set_function("useItem", &Equipment::useItem, this);

		script.initClasses<WearableItem, UsableItem>();
	}

	void Equipment::init() {
		auto& gui = getModule<GUI>(); 

		gui.addTexture("Assets/Equipment/ringsSlider.png");
		gui.addTexture("Assets/Test/ScrollerBound.png");
		gui.addTexture("Assets/Test/ScrollerBar.png");
		gui.addTexture("Assets/Test/Scroller.png");
		gui.addTexture("Assets/Equipment/slot.png");
		gui.addTexture("Assets/Equipment/chosenSlot.png");
		gui.addTexture("Assets/Equipment/highlight.png");
		gui.addTexture("Assets/Equipment/leftArrow.png");
		gui.addTexture("Assets/Equipment/rightArrow.png");
		gui.addFont("Assets/Equipment/NotoMono.ttf");
		gui.addTexture("Assets/Equipment/szczegoly.png");
		gui.addTexture("Assets/Equipment/cancel.png");
		gui.addTexture("Assets/Equipment/rozwijanie.png");

		_base = gui.addInterface();
		_base->setSizingWidthToHeightProportion(1.f);

		_equipmentFrame = new WindowWidget();
		_base->add(_equipmentFrame);
		_equipmentFrame->makeChildrenUnresizable();
		_equipmentFrame->setPropPosition(0.5f, 1.f);		
		_equipmentFrame->setPropSize(.63f, .7f);
		_equipmentFrame->setTexture(gui.getAsset<sf::Texture>("Assets/Equipment/ringsSlider.png"), 180);
	
		_hideButton = new ImageWidget;
		_equipmentFrame->add(_hideButton);
		_hideButton->setPropPosition(0.5f, -0.06f);
		_hideButton->setPropSize(0.25f, 0.042f);
		_hideButton->setTexture(gui.getTexture("Assets/Equipment/rozwijanie.png"));
		_hideButton->setCallback(Widget::CallbackType::onRelease, [this](Widget* owner) {
			if (_isEquipmentHidden)
				_openEquipment();
			else
				_closeEquipment();
		});

		_armorSlots = new ArmorSlots({0.11f, 0.11f}, this);
		_armorSlots->setParent(_equipmentFrame);
		_armorSlots->setPropPosition({ 0.15f, 0.08f });
		_armorSlots->initAssetsViaGUI(gui);

		_normalSlots = new NormalSlots(20, gui.getAsset<sf::Texture>("Assets/Equipment/slot.png"), gui.getAsset<sf::Texture>("Assets/Equipment/highlight.png"), { 62, 62 }, this);
		_normalSlots->setParent(_equipmentFrame);
		_normalSlots->setPropPosition(sf::Vector2f(0.15f, .9f));

		_ringSlider = new RingSlider({ 70u, 70u }, this);
		_ringSlider->setParent(_equipmentFrame);
		_ringSlider->initAssetsViaGUI(gui);
		_ringSlider->setPropPosition({ 0.9f, 0.4f });

		_itemPreview = new ItemPreview(gui.getAsset<sf::Texture>("Assets/Equipment/slot.png"), gui.getAsset<sf::Texture>("Assets/Equipment/szczegoly.png"), gui.getAsset<sf::Font>("Assets/Equipment/NotoMono.ttf"));
		_itemPreview->setParent(_base);
		_itemPreview->minimalize();

		_replaceItem = new ReplaceItem(gui.getAsset<sf::Texture>("Assets/Equipment/slot.png"), gui.getAsset<sf::Texture>("Assets/Equipment/szczegoly.png"), gui.getAsset<sf::Texture>("Assets/Equipment/cancel.png"), gui.getAsset<sf::Font>("Assets/Equipment/NotoMono.ttf"), this);
		_replaceItem->setParent(_base);
		_replaceItem->close();

		_itemManager = new ItemManager;
		_itemManager->setNewPath("Assets/Equipment/items.json");
		_listOfObjects = _itemManager->loadFromFile(getModule<Script>());

		_equipmentPosition = _equipmentFrame->getPosition();
	}

	void Equipment::update(float deltaTime) {
		_normalSlots->update(deltaTime);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab) && _isEquipmentHidden) {
			_openEquipment();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab) && !_isEquipmentHidden) {
			_closeEquipment();
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
			if (_isReplacing) {
				_replaceItem->higherPosition();
			}
		}
	}

	void Equipment::disableItemPreview() {
		_itemPreview->minimalize();
		_isPreviewOn = false;
		if (_isReplacing) {
			_replaceItem->lowerPosition();
		}
	}

	UsableItem* Equipment::getUsableItem(const std::string& nameId) {
		UsableItem* temp = new UsableItem(nameId);
		*temp = *dynamic_cast<UsableItem*>(_listOfObjects.find(nameId)->second);
		if (_listOfObjects.find(nameId) != _listOfObjects.end()) {
			return temp;
		}
		return nullptr;
	}

	WearableItem* Equipment::getWearableItem(const std::string& nameId) {
		WearableItem* temp = new WearableItem(nameId);
		*temp = *dynamic_cast<WearableItem*>(_listOfObjects.find(nameId)->second);
		if (_listOfObjects.find(nameId) != _listOfObjects.end()) {
			return temp;
		}
		return nullptr;
	}

	bool Equipment::addUsableItem(UsableItem* item) {
		if (_normalSlots->addItem(item))
			return true;
		else {
			_replacingStatus = statusOfEq::waiting;
			return false;
		}
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
	//odtad
	void Equipment::_replaceNewItem(EquipmentObject* item) {
		_replaceItem->setItem(item);
		_isReplacing = true;
		if (!_isPreviewOn) {
			_replaceItem->lowerPosition();
		}
	}

	void Equipment::_stopReplacingItem(bool hasBeenSuccesfull) {
		_normalSlots->_stopReplacing();
		_isReplacing = false;
		_replaceItem->close();
		if (hasBeenSuccesfull)
			_replacingStatus = statusOfEq::replaced;
		else
			_replacingStatus = statusOfEq::notReplaced;
	}

	statusOfEq Equipment::lastChangeStatus() {
		return _replacingStatus;
	}

	void Equipment::_openEquipment() {
		_equipmentFrame->setPropPosition({ 0.5f, 1.f }, { .2f, gui::Easing::EaseOutExpo , [this]() {
			_isEquipmentHidden = false; 
			_replaceItem->maximize();
		} });
	}
	void Equipment::_closeEquipment() {
		_replaceItem->minimalize();
		_equipmentFrame->setPropPosition({ 0.5f, 3.35f }, { .2f, gui::Easing::EaseOutExpo , [this]() {
			_isEquipmentHidden = true;
		} });
	}
	//dotad
	bool Equipment::_isEquipmentOpen() {
		return !_isEquipmentHidden;
	}
	bool Equipment::hasChosenAmulet(const std::string& nameId) {
		return _armorSlots->getChosenAmulet()->getNameId() == nameId;
	}
	bool Equipment::hasArmor(const std::string& nameId) {
		return _armorSlots->getArmorSlot()->getItem()->getNameId() == nameId;
	}
	bool Equipment::hasWeapon(const std::string& nameId) {
		return _armorSlots->getWeaponSlot()->getItem()->getNameId() == nameId;
	}
	bool Equipment::useItem(const std::string& nameId) {
		if (_normalSlots->hasItem(nameId)) {
			return _normalSlots->useItem(nameId);
		}
		return false;
	}
	bool Equipment::removeUsableItem(const std::string& nameId) {
		return _normalSlots->removeItem(nameId);
	}
	bool Equipment::removeUsableItem(const std::string& nameId, int quantity) {
		return _normalSlots->removeItem(nameId, quantity);
	}
	bool Equipment::removeAllItems(const std::string& nameId) {
		return _normalSlots->removeItem(nameId, 0);
	}
	bool Equipment::_hasItem(const std::string& nameId) {
		return _normalSlots->hasItem(nameId);
	}
	bool Equipment::_hasItem(const std::string& nameId, int quantity) {
		return _normalSlots->hasItem(nameId, quantity);
	}
}