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
#include "Necklace.hpp"

#include "ItemManager.hpp"
#include "Szczur/Modules/GUI/InterfaceWidget.hpp"

#include "Szczur/Modules/GUI/Animation/Anim.hpp"

namespace rat {
	Equipment::Equipment()
	: _mainWindow(getModule<Window>()), _window(_mainWindow.getWindow()), _input(getModule<Input>())
	{
		LOG_INFO("Initializing Equipment module");
	
		initScript();
		init();		
		LOG_INFO("Module Equipment initialized");
		getModule<Script>().scriptFile("Assets/Equipment/test.lua");
	}

	Equipment::~Equipment() {
		LOG_INFO("Module Input destructed");
	}

	void Equipment::initScript() {
		Script& script = getModule<Script>();
		auto module = script.newModule("Equipment");

		module.set("NOT_REPLACED", StatusOfEq::notReplaced);
		module.set("REPLACED", StatusOfEq::replaced);
		module.set("WAITING", StatusOfEq::waiting);

		module.set_function("addUsableItem", &Equipment::addUsableItem, this);
		module.set_function("addWearableItem", &Equipment::addWearableItem, this);
		module.set_function("getUsableItem", &Equipment::getUsableItem, this);
		module.set_function("getWearableItem", &Equipment::getWearableItem, this);
		module.set_function("getItemTable", &Equipment::getItemsList, this);
		module.set_function("getFreeSlotsAmount", &Equipment::getFreeSlotsAmount, this);
		module.set_function("resizeSlots", &Equipment::resizeSlots, this);
		module.set_function("getSlotsAmount", &Equipment::getSlotsAmount, this);
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
		
		module.set_function("useItem", &Equipment::useItem, this);
		module.set_function("hasChosenStone", &Equipment::hasChosenStone, this);

		script.initClasses<WearableItem, UsableItem>();
	}

	void Equipment::init() {
		auto& gui = getModule<GUI>(); 

		gui.addTexture("Assets/Equipment/Okno.png");
		gui.addTexture("Assets/Equipment/Zbroja.png");
		gui.addTexture("Assets/Equipment/Miecz.png");
		gui.addTexture("Assets/Equipment/Naszyjnik.png");
		gui.addTexture("Assets/Equipment/shadow.png");
		gui.addTexture("Assets/Equipment/szczegoly.png");
		gui.addTexture("Assets/Equipment/background.png");
		gui.addTexture("Assets/Equipment/ringsSlider.png");
		gui.addTexture("Assets/Equipment/slot.png");
		gui.addFont("Assets/Equipment/NotoMono.ttf");
		gui.addTexture("Assets/Equipment/cancel.png");
		gui.addTexture("Assets/Equipment/rozwijanie.png");

		_base = gui.addInterface();
		_base->setSizingWidthToHeightProportion(1.f);

		_background = new ImageWidget;
		_base->add(_background);
		_background->setTexture(gui.getTexture("Assets/Equipment/background.png"));
		_background->setPropSize(1.78f, 1.f);

		_equipmentFrame = new ImageWidget;
		_base->add(_equipmentFrame);
		_equipmentFrame->makeChildrenUnresizable();
		_equipmentFrame->setPropPosition(0.5f, 3.33f);
		_equipmentFrame->setPropSize(.564f, .689f);
		_equipmentFrame->setTexture(gui.getAsset<sf::Texture>("Assets/Equipment/Okno.png"));
	
		_armorImage = new ImageWidget;
		_equipmentFrame->add(_armorImage);
		_armorImage->setPropSize(.262f, .836f);
		_armorImage->setPropPosition(-0.4f, .31f);
		_armorImage->setTexture(gui.getTexture("Assets/Equipment/Zbroja.png"));

		_weaponImage = new ImageWidget;
		_equipmentFrame->add(_weaponImage);
		_weaponImage->setPropSize(.454f, .99f);
		_weaponImage->setPropPosition(2.95f, .43f);
		_weaponImage->setTexture(gui.getTexture("Assets/Equipment/Miecz.png"));

		_necklace = new Necklace(gui.getTexture("Assets/Equipment/Naszyjnik.png"), this);
		_necklace->setParent(_equipmentFrame);
		_necklace->setPropPosition(1.f, .0f);
		_necklace->setPropSize(0.42f, 0.14f);

		/*_armorSlots = new ArmorSlots({0.11f, 0.11f}, this);
		_armorSlots->setParent(_equipmentFrame);
		_armorSlots->setPropPosition({ 0.15f, 0.08f });
		_armorSlots->initAssetsViaGUI(gui);*/

		_normalSlots = new NormalSlots(20, gui.getAsset<sf::Texture>("Assets/Equipment/slot.png"), gui.getAsset<sf::Texture>("Assets/Equipment/shadow.png"), this, {.068f, 0.091f});
		_normalSlots->setParent(_equipmentFrame);
		_normalSlots->setPropPosition(sf::Vector2f(0.37f, .69f));

		_ringSlider = new RingSlider(this, gui, { .068f, 0.091f });
		_ringSlider->setParent(_equipmentFrame);
		_ringSlider->setPropPosition({ 0.97f, .9f });

		_itemPreview = new ItemPreview(gui.getAsset<sf::Texture>("Assets/Equipment/slot.png"), gui.getAsset<sf::Texture>("Assets/Equipment/szczegoly.png"), gui.getAsset<sf::Font>("Assets/Equipment/NotoMono.ttf"));
		_itemPreview->setParent(_base);
		_itemPreview->minimalize();

		_replaceItem = new ReplaceItem(gui.getAsset<sf::Texture>("Assets/Equipment/slot.png"), gui.getAsset<sf::Texture>("Assets/Equipment/szczegoly.png"), gui.getAsset<sf::Texture>("Assets/Equipment/cancel.png"), gui.getAsset<sf::Font>("Assets/Equipment/NotoMono.ttf"), this);
		_replaceItem->setParent(_base);
		_replaceItem->close();

		_itemManager = new ItemManager;
		_pathToJson = "Assets/Equipment/items.json";
		_itemManager->setNewPath(_pathToJson);
		_listOfObjects = _itemManager->loadFromFile(getModule<Script>());

		_equipmentPosition = _equipmentFrame->getPosition();
		_openEquipment();
	}

	void Equipment::update(float deltaTime) {
		_normalSlots->update(deltaTime);

		if (_input.getManager().isPressed(rat::Keyboard::I) && _isEquipmentHidden) {
			_openEquipment();
		}
		if (_input.getManager().isPressed(rat::Keyboard::I) && !_isEquipmentHidden) {
			_closeEquipment();
		}
	}

	//when play button in pressed and we activate equipment
	void Equipment::startEquipment() {
		_base->fullyActivate();
		
	}

	//when stop button is pressed and we deactivate equipment
	void Equipment::stopEquipment() {
		_base->fullyDeactivate();
		_armorSlots->reset();
		_necklace->reset();
		_ringSlider->reset();
		_normalSlots->reset();
		_closeEquipment();
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
			_replacingStatus = StatusOfEq::waiting;
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
		case equipmentObjectType::stone:
			_ringSlider->addItem(item);
			break;
		}
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
		for (auto& it : _ringSlider->getStonesList())
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
	
	void Equipment::setNewItemsPath(std::string newPath){
		_pathToJson = newPath;
		_itemManager->setNewPath(newPath);
	}
	void Equipment::reloadItemsList() {
		_listOfObjects = _itemManager->loadFromFile(getModule<Script>());
	}

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
			_replacingStatus = StatusOfEq::replaced;
		else
			_replacingStatus = StatusOfEq::notReplaced;
	}

	StatusOfEq Equipment::lastChangeStatus() {
		return _replacingStatus;
	}

	void Equipment::_openEquipment() {
		_equipmentFrame->setPropPositionInTime({ 0.1f, 0.4f }, { .2f, gui::Easing::EaseOutQuad , [this]() {
			_isEquipmentHidden = false; 
			_replaceItem->maximize();
		} });
		_equipmentFrame->fullyActivate();
	}
	void Equipment::_closeEquipment() {
		_replaceItem->minimalize();
		_equipmentFrame->setPropPositionInTime({ -.7f, .4f }, { .2f, gui::Easing::EaseOutQuad , [this]() {
			_isEquipmentHidden = true;
			_equipmentFrame->fullyDeactivate();
		} });
	}
	bool Equipment::_isEquipmentOpen() {
		return !_isEquipmentHidden;
	}
	/*bool Equipment::hasChosenAmulet(const std::string& nameId) {
		return _armorSlots->getChosenAmulet()->getNameId() == nameId;
	}
	bool Equipment::hasArmor(const std::string& nameId) {
		return _armorSlots->getArmorSlot()->getItem()->getNameId() == nameId;
	}
	bool Equipment::hasWeapon(const std::string& nameId) {
		return _armorSlots->getWeaponSlot()->getItem()->getNameId() == nameId;
	}*/

	bool Equipment::hasChosenStone(std::string& nameId) {
		return _necklace->hasStone(nameId);
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

	std::string& Equipment::getPathToJson() {
		return _pathToJson;
	}

	bool Equipment::_stoneStatusChanged(WearableItem* stone, bool status) {
		//we're putting stone on
		if (status) {
			return _necklace->addStone(stone);
		}
		//we're taking stone of
		else {
			_ringSlider->_takeStoneOf(stone);
			return _necklace->removeStone(stone);		
		}
	}
}