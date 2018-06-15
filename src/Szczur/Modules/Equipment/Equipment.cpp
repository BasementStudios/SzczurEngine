#include "Equipment.hpp"
#include "Szczur/Modules/GUI/WindowWidget.hpp"

#include "EquipmentObject.hpp"
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

		_base = gui.addInterface();
		//_base->setPosition(sf::Vector2f(window.getSize().x / 10, window.getSize().y / 10));

		_equipmentFrame = new WindowWidget();
		_base->add(_equipmentFrame);

		_equipmentSlot = new EquipmentSlot;
		_equipmentSlot->setParent(_equipmentFrame);

		EquipmentObject* item = new EquipmentObject("item", "tez item", gui.getAsset<sf::Texture>("Assets/Equipment/item1.png"), equipmentObjectType::amulet);

		_equipmentSlot->setTexture(gui.getAsset<sf::Texture>("Assets/Test/NineMinusPatchTest.png"));
		_equipmentSlot->setSize(60u, 60u);
		_equipmentSlot->setPosition({40, 40});
		_equipmentSlot->setItem(item);

		_equipmentFrame->setPosition(sf::Vector2f(window.getSize().x /10, window.getSize().y /10)); //base equipment widget
		_equipmentFrame->setSize(sf::Vector2u(window.getSize().x / 10 * 8, window.getSize().y / 10 * 8));
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