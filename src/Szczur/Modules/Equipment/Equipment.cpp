#include "Equipment.hpp"
#include "Szczur/Modules/GUI/WindowWidget.hpp"

#include "EquipmentObject.hpp"
#include "NormalSlots.hpp"
#include "EquipmentSlot.hpp"
#include "ArmorSlots.hpp"

namespace rat {
	Equipment::Equipment()
	: mainWindow(getModule<Window>()), window(mainWindow.getWindow())
	{
		mainWindow.pushGLStates();
		_canvas.create(window.getSize().x, window.getSize().y);
		mainWindow.popGLStates();

		init();
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
		gui.addAsset<sf::Texture>("Assets/Equipment/downArrow.png");
		gui.addAsset<sf::Texture>("Assets/Equipment/upArrow.png");

		_base = gui.addInterface();

		_equipmentFrame = new WindowWidget();
		_base->add(_equipmentFrame);

		_equipmentFrame->setPosition(sf::Vector2f(window.getSize().x / 10 * 3, window.getSize().y / 10 * 3.5f)); //base equipment widget
		_equipmentFrame->setSize(sf::Vector2u(window.getSize().x / 10 * 4, window.getSize().y / 10 * 6.5f));
		_equipmentFrame->setTexture(gui.getAsset<sf::Texture>("Assets/Test/NinePatchTest.png"), 70);

		EquipmentObject* _armor = new EquipmentObject("zbroja", "tez item", gui.getAsset<sf::Texture>("Assets/Equipment/zbroja.png"), equipmentObjectType::armor);
		EquipmentObject* _sword = new EquipmentObject("miecz", "tez item", gui.getAsset<sf::Texture>("Assets/Equipment/miecz.png"), equipmentObjectType::sword);
		EquipmentObject* _potion = new EquipmentObject("potion", "tez item", gui.getAsset<sf::Texture>("Assets/Equipment/potion.png"), equipmentObjectType::potion);
		EquipmentObject* _amulet1 = new EquipmentObject("amulet1", "tez item", gui.getAsset<sf::Texture>("Assets/Equipment/amulet1.png"), equipmentObjectType::amulet);
		EquipmentObject* _amulet2 = new EquipmentObject("amulet2", "tez item", gui.getAsset<sf::Texture>("Assets/Equipment/amulet2.png"), equipmentObjectType::amulet);
		EquipmentObject* _scroll = new EquipmentObject("zwoj", "tez item", gui.getAsset<sf::Texture>("Assets/Equipment/zwoj.png"), equipmentObjectType::other);
		

		_normalSlots = new NormalSlots(20, gui.getAsset<sf::Texture>("Assets/Equipment/slot.png"), {60, 60});
		_normalSlots->setParent(_equipmentFrame);
		_normalSlots->addItem(_scroll);
		_normalSlots->addItem(_potion);
		_normalSlots->setPosition(sf::Vector2f(window.getSize().x / 10 * 0.3, window.getSize().y / 4));

		_armorSlots = new ArmorSlots(gui.getAsset<sf::Texture>("Assets/Equipment/slot.png"), {80u, 80u}, gui.getAsset<sf::Texture>("Assets/Equipment/upArrow.png"), gui.getAsset<sf::Texture>("Assets/Equipment/downArrow.png"));
		_armorSlots->setParent(_equipmentFrame);
		_armorSlots->setPosition(sf::Vector2f(window.getSize().x / 10 * 0.3, window.getSize().y / 50));
		_armorSlots->setArmor(_armor);
		_armorSlots->setWeapon(_sword);
		_armorSlots->addAmulet(_amulet1);
		_armorSlots->addAmulet(_amulet2);	
	}

	void Equipment::update(float deltaTime) {
		_normalSlots->update();
	}

	void Equipment::render() {
		mainWindow.pushGLStates();

		_canvas.clear(sf::Color::Transparent);
		_canvas.display();

		mainWindow.getWindow().draw(sf::Sprite(_canvas.getTexture()));

		mainWindow.popGLStates();
	}
}