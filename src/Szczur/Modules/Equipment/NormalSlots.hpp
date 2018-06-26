#pragma once

#include <vector>
#include <SFML/Graphics.hpp>


namespace rat {	
	class EquipmentSlot; class EquipmentObject; class Widget; class ImageWidget; class Equipment;
	typedef std::multimap<sf::String, EquipmentSlot*> itemMap;

	class NormalSlots			//part of equipment for normal items looking like a grid
	{
	public:
		NormalSlots(unsigned int slotNumber, sf::Texture* frameText, sf::Vector2i frameSize, Equipment* equipment);

		void addItem(EquipmentObject* item);
		void removeItem(sf::String itemName);
		void resizeSlots(unsigned int newSize);
		void setParent(Widget* newBase);

		void setPosition(sf::Vector2f);
		sf::Vector2f getPosition();

		void update();		
	private:
		unsigned int _slotNumber;
		sf::Vector2i _frameSize;

		Widget* _base;

		sf::Texture* _frameText;

		itemMap _itemSlots;		//slots with items
		std::vector<EquipmentSlot*> _freeSlots;

		//std::pair<bool, EquipmentSlot*> isMouseOverSlot(sf::Vector2i position, bool freeSlot);
		std::shared_ptr<EquipmentSlot> _slotHeld;
		std::shared_ptr<EquipmentSlot> _slotDropped;
		EquipmentObject* _itemHeld;
		ImageWidget* _itemHeldWidget;

		bool _isMouseButtonHeld;
		sf::Vector2i _originalMousePosition;

		void onMouseButtonPressed(std::shared_ptr<EquipmentSlot> clickedObj);
		void onMouseButtonReleased();
	};
}
