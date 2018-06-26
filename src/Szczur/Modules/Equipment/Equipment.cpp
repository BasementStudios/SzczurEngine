#include "Equipment.hpp"
#include "Szczur/Modules/GUI/WindowWidget.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"

#include "EquipmentObject.hpp"
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

		EquipmentObject* _armor = new EquipmentObject("zbroja", "tez item", gui.getAsset<sf::Texture>("Assets/Equipment/zbroja.png"), equipmentObjectType::armor);
		EquipmentObject* _sword = new EquipmentObject("miecz", "tez item", gui.getAsset<sf::Texture>("Assets/Equipment/miecz.png"), equipmentObjectType::sword);
		EquipmentObject* _potion = new EquipmentObject("potion", "tez item", gui.getAsset<sf::Texture>("Assets/Equipment/potion.png"), equipmentObjectType::potion);
		EquipmentObject* _amulet1 = new EquipmentObject("amulet1", "tez item", gui.getAsset<sf::Texture>("Assets/Equipment/amulet1.png"), equipmentObjectType::amulet);
		EquipmentObject* _amulet2 = new EquipmentObject("amulet2", "tez item", gui.getAsset<sf::Texture>("Assets/Equipment/amulet2.png"), equipmentObjectType::amulet);
		EquipmentObject* _scroll = new EquipmentObject("zwoj", "tez item ale ten ma bardzo dlugi tekst", gui.getAsset<sf::Texture>("Assets/Equipment/zwoj.png"), equipmentObjectType::other);

		_armorSlots = new ArmorSlots(gui.getAsset<sf::Texture>("Assets/Equipment/slot.png"), {100u, 100u}, gui.getAsset<sf::Texture>("Assets/Equipment/leftArrow.png"), gui.getAsset<sf::Texture>("Assets/Equipment/rightArrow.png"), this);
		_armorSlots->setParent(_equipmentFrame);
		_armorSlots->setPosition(sf::Vector2f(window.getSize().x / 10 * 0.3, window.getSize().y / 20));
		_armorSlots->setArmor(_armor);
		_armorSlots->setWeapon(_sword);
		_armorSlots->addAmulet(_amulet1);
		_armorSlots->addAmulet(_amulet2);

		_normalSlots = new NormalSlots(20, gui.getAsset<sf::Texture>("Assets/Equipment/slot.png"), { 62, 62 }, this);
		_normalSlots->setParent(_base);
		_normalSlots->addItem(_scroll);
		_normalSlots->addItem(_potion);
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
		_itemPreview->setItem(item);
		isPreviewOn = true;
	}

	void Equipment::disableItemPreview() {
		_itemPreview->minimalize();
		isPreviewOn = false;
	}
}