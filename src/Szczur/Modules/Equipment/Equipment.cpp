#include "Equipment.hpp"
#include "Szczur/Modules/GUI/WindowWidget.hpp"

#include "EquipmentObject.hpp"
#include "NormalSlots.hpp"
#include "EquipmentSlot.hpp"

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
		gui.addAsset<sf::Texture>("Assets/Test/NineMinusPatchTest.png");
		gui.addAsset<sf::Texture>("Assets/Equipment/item1.png");
		gui.addAsset<sf::Texture>("Assets/Equipment/item2.png");

		_base = gui.addInterface();

		_equipmentFrame = new WindowWidget();
		_base->add(_equipmentFrame);

		EquipmentObject* item = new EquipmentObject("item", "tez item", gui.getAsset<sf::Texture>("Assets/Equipment/item1.png"), equipmentObjectType::amulet);
		EquipmentObject* item2 = new EquipmentObject("item", "tez item", gui.getAsset<sf::Texture>("Assets/Equipment/item2.png"), equipmentObjectType::amulet);

		_normalSlots = new NormalSlots(15, gui.getAsset<sf::Texture>("Assets/Test/NineMinusPatchTest.png"), {70u, 70u});
		_normalSlots->setParent(_equipmentFrame);
		_normalSlots->addItem(item);
		_normalSlots->addItem(item2);
		_normalSlots->setPosition(sf::Vector2f(window.getSize().x / 10 * 0.3, window.getSize().y / 10 * 2.5));

		_equipmentFrame->setPosition(sf::Vector2f(window.getSize().x /10 * 2.5f, window.getSize().y /10 * 4)); //base equipment widget
		_equipmentFrame->setSize(sf::Vector2u(window.getSize().x / 10 * 5, window.getSize().y / 10 * 6));
		_equipmentFrame->setTexture(gui.getAsset<sf::Texture>("Assets/Test/NinePatchTest.png"), 70);

	}

	void Equipment::update(float deltaTime) {

	}

	void Equipment::render() {
		auto& mainWindow = getModule<Window>();

		mainWindow.pushGLStates();

		_canvas.clear(sf::Color::Transparent);
		_canvas.display();

		mainWindow.getWindow().draw(sf::Sprite(_canvas.getTexture()));

		mainWindow.popGLStates();
	}
}