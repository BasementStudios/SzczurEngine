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

namespace rat {
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

		/*WearableItem* item = new WearableItem("aaa");
		item->setName("aaa");
		item->setIcon("Assets/Equipment/amulet1.png");
		item->setType(equipmentObjectType::weapon);
		_listOfObjects.insert(std::make_pair(("aaa"), item));

		addWearableItem(item);*/
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

		script.initClasses<WearableItem, UsableItem>();
		//module.set_function("a", &NewModule::a, this);
		//module.set_function("functionName", [&](int a, int b){return a+b;});
		//auto object = script.newClass<NewClass>("NewClass", "NewModule", "../NewClass.lua");
	}

	void Equipment::init() {
		auto& gui = getModule<GUI>(); 

		_base = gui.addInterface();

		_equipmentFrame = new WindowWidget();
		_base->add(_equipmentFrame);

		//_equipmentFrame->setPosition(sf::Vector2f(_window.getSize().x / 10 * 3, _window.getSize().y / 10 * 3.5f)); //base equipment widget
		//_equipmentFrame->setPropPosition(0.5f, 1.f);	
		_equipmentFrame->setPosition(384.f, 220.f); //{384.f ,220.f }
		_equipmentFrame->setPropSize(.4f, .5f);
		_equipmentFrame->setTexture(gui.getAsset<sf::Texture>("Assets/Test/NinePatchTest.png"), 200);
	
		_armorSlots = new ArmorSlots(gui.getAsset<sf::Texture>("Assets/Equipment/slot.png"), {90u, 90u}, gui.getAsset<sf::Texture>("Assets/Equipment/leftArrow.png"), gui.getAsset<sf::Texture>("Assets/Equipment/rightArrow.png"), this);
		_armorSlots->setParent(_equipmentFrame);
		_armorSlots->setPropPosition({0.0f, 0.0f});

		_normalSlots = new NormalSlots(20, gui.getAsset<sf::Texture>("Assets/Equipment/slot.png"), { 62, 62 }, this);
		_normalSlots->setParent(_equipmentFrame);
		_normalSlots->setPropPosition(sf::Vector2f(0.1f, 0.95f));

		_ringSlider = new RingSlider(gui.getAsset<sf::Texture>("Assets/Equipment/slot.png"), { 70u, 70u }, gui.getAsset<sf::Texture>("Assets/Test/ScrollerBar.png"), gui.getAsset<sf::Texture>("Assets/Test/ScrollerBound.png"), gui.getAsset<sf::Texture>("Assets/Test/Scroller.png"), this, gui.getAsset<sf::Texture>("Assets/Equipment/slot.png"), _window.getSize().y / 10 * 8.f);
		_ringSlider->setParent(_equipmentFrame);
		_ringSlider->setPropPosition({ 1.5f, 0.0f });

		_itemPreview = new ItemPreview(gui.getAsset<sf::Texture>("Assets/Equipment/szczegoly.png"), {270, 90}, gui.getAsset<sf::Font>("Assets/Equipment/NotoMono.ttf"));
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

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::I) && _isEquipmentHidden && _equipmentFrame->getPosition().y == _window.getSize().y) {
			LOG_INFO("1");
			_equipmentFrame->setPositionInTime({ 384.f ,220.f }, .2f);
			_isEquipmentHidden = false;
			_equipmentFrame->fullyActivate();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::I) && !_isEquipmentHidden && _equipmentFrame->getPosition().y == _equipmentPosition.y) {
			LOG_INFO("2");
			_equipmentFrame->setPositionInTime({ _equipmentFrame->getPosition().x, static_cast<float>(_window.getSize().y) }, .2f);
			_isEquipmentHidden = true;			
		}
		if(_equipmentFrame->getPosition().y == _window.getSize().y && _equipmentFrame->isActivated() && !sf::Keyboard::isKeyPressed(sf::Keyboard::I))
			_equipmentFrame->fullyDeactivate();
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

	//void Equipment::createItem(std::string nameId, std::string name, std::string description, std::string iconPath, equipmentObjectType type, bool isUseble) {
	//	auto& gui = getModule<GUI>();
	//	gui.addAsset<sf::Texture>(iconPath);		
	//	EquipmentObject* newObject = new EquipmentObject(nameId, name, description, gui.getAsset<sf::Texture>(iconPath), type, isUseble);
	//	_listOfObjects[nameId] = newObject;	//cos z tymi jsonami
	//}

	/*UsableItem* Equipment::createUsableItem(std::string nameId) {
		UsableItem* temp = new UsableItem(nameId);
		_listOfObjects[nameId] = temp;
		temp->initScript(getModule<Script>());
		return temp;
	}

	WearableItem* Equipment::createWearableItem(std::string nameId) {
		WearableItem* temp = new WearableItem(nameId);
		_listOfObjects[nameId] = temp;
		temp->initScript(getModule<Script>());
		return temp;
	}*/

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
		sol::table _table = getModule<Script>().get().create_table();	//todo: add rings
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
}