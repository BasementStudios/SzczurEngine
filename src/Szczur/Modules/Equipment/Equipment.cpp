#include "Equipment.hpp"
#include "Szczur/Modules/GUI/WindowWidget.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"

#include "NormalSlots.hpp"
#include "EquipmentSlot.hpp"
#include "ArmorSlots.hpp"
#include "ItemPreview.hpp"

namespace rat {
	Equipment::Equipment()
	: mainWindow(getModule<Window>()), window(mainWindow.getWindow())
	{
		mainWindow.pushGLStates();
		_canvas.create(window.getSize().x, window.getSize().y);
		mainWindow.popGLStates();
	
		init();
		initScript();
	}

	void Equipment::initScript() {
		Script& script = getModule<Script>();
		auto module = script.newModule("Equipment");

		module.set("ITEM_TYPE_WEAPON", equipmentObjectType::weapon);
		module.set("ITEM_TYPE_ARMOR", equipmentObjectType::armor);
		module.set("ITEM_TYPE_AMULET", equipmentObjectType::amulet);
		module.set("ITEM_TYPE_RING", equipmentObjectType::ring);
		module.set("ITEM_TYPE_OTHER", equipmentObjectType::other);

		module.set_function("createItem", &Equipment::createItem, this);

		module.set_function("addItem", &Equipment::addItem, this);
		module.set_function("removeItem", &Equipment::removeItem, this);
		module.set_function("getItemTable", &Equipment::getItemsList, this);
		module.set_function("getFreeSlotsAmount", &Equipment::getFreeSlotsAmount, this);
		module.set_function("resizeSlots", &Equipment::resizeSlots, this);
		module.set_function("getSlotsAmount", &Equipment::getSlotsAmount, this);

		script.scriptFile("test.lua");

		//module.set_function("a", &NewModule::a, this);
		//module.set_function("functionName", [&](int a, int b){return a+b;});
	}

	void Equipment::init() {
		auto& gui = getModule<GUI>(); 

		gui.addAsset<sf::Texture>("Assets/Test/NinePatchTest.png");
		gui.addAsset<sf::Texture>("Assets/Equipment/slot.png");
		gui.addAsset<sf::Texture>("Assets/Equipment/zbroja.png");
		gui.addAsset<sf::Texture>("Assets/Equipment/miecz.png");
		gui.addAsset<sf::Texture>("Assets/Equipment/amulet1.png");
		gui.addAsset<sf::Texture>("Assets/Equipment/amulet2.png");
		gui.addAsset<sf::Texture>("Assets/Equipment/zwoj.png");
		gui.addAsset<sf::Texture>("Assets/Equipment/potion.png");
		gui.addAsset<sf::Texture>("Assets/Equipment/rightArrow.png");
		gui.addAsset<sf::Texture>("Assets/Equipment/leftArrow.png");
		gui.addAsset<sf::Texture>("Assets/Equipment/szczegoly.png");
		gui.addAsset<sf::Font>("Assets/Equipment/NotoMono.ttf");

		_base = gui.addInterface();

		_equipmentFrame = new WindowWidget();
		_base->add(_equipmentFrame);

		_equipmentFrame->setPosition(sf::Vector2f(window.getSize().x / 10 * 3, window.getSize().y / 10 * 3.5f)); //base equipment widget
		_equipmentFrame->setSize(sf::Vector2u(window.getSize().x / 10 * 4, window.getSize().y / 10 * 6.5f));
		_equipmentFrame->setTexture(gui.getAsset<sf::Texture>("Assets/Test/NinePatchTest.png"), 70);

		EquipmentObject* _armor = new EquipmentObject("zbroja", "zbroja", "tez item", gui.getAsset<sf::Texture>("Assets/Equipment/zbroja.png"), equipmentObjectType::armor);
		EquipmentObject* _sword = new EquipmentObject("zbroja", "miecz", "tez item", gui.getAsset<sf::Texture>("Assets/Equipment/miecz.png"), equipmentObjectType::weapon);
		EquipmentObject* _potion = new EquipmentObject("zbroja", "potion", "tez item", gui.getAsset<sf::Texture>("Assets/Equipment/potion.png"), equipmentObjectType::other);
		EquipmentObject* _amulet1 = new EquipmentObject("zbroja", "amulet1", "tez item", gui.getAsset<sf::Texture>("Assets/Equipment/amulet1.png"), equipmentObjectType::amulet);
		EquipmentObject* _amulet2 = new EquipmentObject("zbroja", "amulet2", "tez item", gui.getAsset<sf::Texture>("Assets/Equipment/amulet2.png"), equipmentObjectType::amulet);
		EquipmentObject* _scroll = new EquipmentObject("zbroja", "zwoj", "tez item ale ten ma bardzo dlugi tekst", gui.getAsset<sf::Texture>("Assets/Equipment/zwoj.png"), equipmentObjectType::other);

		_armorSlots = new ArmorSlots(gui.getAsset<sf::Texture>("Assets/Equipment/slot.png"), {100u, 100u}, gui.getAsset<sf::Texture>("Assets/Equipment/leftArrow.png"), gui.getAsset<sf::Texture>("Assets/Equipment/rightArrow.png"), this);
		_armorSlots->setParent(_equipmentFrame);
		_armorSlots->setPosition(sf::Vector2f(window.getSize().x / 10 * 0.3, window.getSize().y / 20));

		_normalSlots = new NormalSlots(20, gui.getAsset<sf::Texture>("Assets/Equipment/slot.png"), { 62, 62 }, this);
		_normalSlots->setParent(_base);
		_normalSlots->setPosition(sf::Vector2f(window.getSize().x / 10 * 3.4, window.getSize().y / 1.6f));

		_itemPreview = new ItemPreview(gui.getAsset<sf::Texture>("Assets/Equipment/szczegoly.png"), {270, 90}, gui.getAsset<sf::Font>("Assets/Equipment/NotoMono.ttf"));
		_itemPreview->setParent(_base);
		_itemPreview->minimalize();
	}

	void Equipment::update(float deltaTime) {
		_normalSlots->update();
		if (isPreviewOn)
			_itemPreview->setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));
	}

	void Equipment::render() {
		mainWindow.pushGLStates();

		_canvas.clear(sf::Color::Transparent);
		_canvas.display();

		mainWindow.getWindow().draw(sf::Sprite(_canvas.getTexture()));

		mainWindow.popGLStates();
	}

	void Equipment::enableItemPreview(EquipmentObject* item) {
		if (canPreviewBeInstantiated) {
			_itemPreview->setItem(item);
			isPreviewOn = true;
		}
	}

	void Equipment::disableItemPreview() {
		_itemPreview->minimalize();
		isPreviewOn = false;
	}

	void Equipment::createItem(std::string nameId, std::string name, std::string description, std::string iconPath, equipmentObjectType type) {
		auto& gui = getModule<GUI>();
		gui.addAsset<sf::Texture>(iconPath);		
		EquipmentObject* newObject = new EquipmentObject(nameId, name, description, gui.getAsset<sf::Texture>(iconPath), type);
		_listOfObjects[nameId] = newObject;	//cos z tymi jsonami
	}

	void Equipment::setCallback(std::string nameId, sol::function firstCallback) {

	}

	void Equipment::setCallback(std::string nameId, sol::function firstCallback, sol::function secondCallback) {

	}

	bool Equipment::addItem(std::string nameId) {
		auto item = _listOfObjects.find(nameId);
		if (item != _listOfObjects.end()) {
			switch (item->second->getType()) {
			case equipmentObjectType::other:
				_normalSlots->addItem(item->second);
				return true;
			case equipmentObjectType::armor:
				_armorSlots->setArmor(item->second);
				return true;
			case equipmentObjectType::weapon:
				_armorSlots->setWeapon(item->second);
				return true;
			case equipmentObjectType::amulet:
				_armorSlots->addAmulet(item->second);
				return true;
			case equipmentObjectType::ring:
				//todo
				return true;
			}
		}
		else {
			return false;
		}
	}

	bool Equipment::removeItem(std::string nameId) {
		auto item = _listOfObjects.find(nameId);
		if (item != _listOfObjects.end()) {
			switch (item->second->getType()) {
			case equipmentObjectType::other:			
				return _normalSlots->removeItem(item->first);
			case equipmentObjectType::armor:
				return false;		//you cannot remove either weapon or armor
			case equipmentObjectType::weapon:
				return false;
			case equipmentObjectType::amulet:
				return _armorSlots->removeAmulet(item->first);
			case equipmentObjectType::ring:
				//todo
				return true;
			}
		}
		return false;
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
	/*
	void setRingsLimit(int newCapacity);*/
}