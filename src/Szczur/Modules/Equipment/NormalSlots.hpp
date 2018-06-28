#pragma once

#include <vector>
#include <SFML/Graphics.hpp>


namespace rat {	
	class EquipmentSlot; class UsebleItem; class Widget; class ImageWidget; class Equipment;
	typedef std::multimap<std::string, EquipmentSlot*> itemMap_t;

	class NormalSlots			//part of equipment for normal items looking like a grid
	{
	public:
		NormalSlots(unsigned int slotNumber, sf::Texture* frameText, sf::Vector2i frameSize, Equipment* equipment);

		void addItem(UsebleItem* item);
		bool removeItem(sf::String itemName);
		void resizeSlots(size_t newSize);
		void setParent(Widget* newBase);
		itemMap_t getItemMap();
		int getFreeSlotsAmount();
		int getSlotsAmount();

		void setPosition(sf::Vector2f);
		sf::Vector2f getPosition();

		void update(float deltaTime);
	private:
		unsigned int _slotAmount;
		sf::Vector2i _frameSize;

		Widget* _base;

		sf::Texture* _frameText;

		itemMap_t _itemSlots;		//slots with items
		std::vector<EquipmentSlot*> _freeSlots;
		std::vector<EquipmentSlot*> _allSlots;

		//std::pair<bool, EquipmentSlot*> isMouseOverSlot(sf::Vector2i position, bool freeSlot);
		std::shared_ptr<EquipmentSlot> _slotHeld;
		std::shared_ptr<EquipmentSlot> _slotDropped;
		UsebleItem* _itemHeld;
		ImageWidget* _itemHeldWidget;

		bool _isMouseButtonHeld;
		sf::Vector2i _originalMousePosition;

		void onMouseButtonPressed(std::shared_ptr<EquipmentSlot> clickedObj);
		void onMouseButtonReleased();

		Equipment* _equipment;

		void checkForDoubleClick(float deltaTime);
		bool _isLeftMouseButtonPressed;
		bool _isCountingToDoubleClickEnabled;	//used in detection of doubleclick
		float _timeFromLastClick;
	};
}
