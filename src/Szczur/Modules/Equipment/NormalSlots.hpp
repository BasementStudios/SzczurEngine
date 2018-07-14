#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "UsableItem.hpp"
#include <queue>

namespace rat {	
	class EquipmentSlot; class UsebleItem; class Widget; class ImageWidget; class Equipment; class ReplaceItem; class ListWidget;
	typedef std::multimap<std::string, std::shared_ptr<EquipmentSlot>> itemMap_t;

	template<class T, class U>
	class SlotsPriorityQueue : public std::priority_queue<T, std::vector<T>, U>
	{
	public:

		bool remove(const T& value) {
			auto it = std::find(this->c.begin(), this->c.end(), value);
			if (it != this->c.end()) {
				this->c.erase(it);
				std::make_heap(this->c.begin(), this->c.end(), this->comp);
				return true;
			}
			else {
				return false;
			}
		}
	};

	struct SortByIndex {
		bool operator () (const std::shared_ptr<EquipmentSlot> lhs, const std::shared_ptr<EquipmentSlot> rhs) const;
	};

	class NormalSlots			//part of equipment for normal items looking like a grid
	{
		friend class Equipment;
	public:
		NormalSlots(unsigned int slotNumber, sf::Texture* frameText, sf::Texture* shadowText, Equipment* equipment, sf::Vector2f frameSize);

		bool addItem(EquipmentObject* item);
		bool removeItem(const std::string& itemNameId);
		bool removeItem(const std::string& itemNameId, int quantity);
		bool removeItem(size_t index, bool deletePermamently);

		bool hasItem(const std::string& itemNameId);
		bool hasItem(const std::string& itemNameId, int quantity);

		void resizeSlots(size_t newSize);
		void setParent(Widget* newBase);
		itemMap_t getItemMap();
		int getFreeSlotsAmount();
		int getSlotsAmount();

		void setPropPosition(sf::Vector2f);
		sf::Vector2f getPosition();

		bool useItem(const std::string& nameId);

		void update(float deltaTime);
		void reset();

	private:
		unsigned int _slotAmount;

		Widget* _base;

		void _removeSlotDropped(std::shared_ptr<EquipmentSlot>);

		sf::Texture* _frameText;

		itemMap_t _occupiedSlots;		//slots with items
		SlotsPriorityQueue<std::shared_ptr<EquipmentSlot>, SortByIndex> _freeSlots;
		ListWidget* _verticalListOfLists;
		std::vector<ListWidget*> _slotLists;
		std::vector<std::shared_ptr<EquipmentSlot>> _allSlots;

		std::shared_ptr<EquipmentSlot> _slotHeld;
		std::shared_ptr<EquipmentSlot> _slotDropped;
		EquipmentObject* _itemHeld;
		ImageWidget* _itemHeldWidget;

		EquipmentObject* _itemForReplacing;

		sf::Vector2i _originalMousePosition;

		void _onMouseButtonPressed(std::shared_ptr<EquipmentSlot> clickedObj);
		void _onMouseButtonReleased();

		Equipment* _equipment;

		void _checkForDoubleClick(float& deltaTime);
		bool _isLeftMouseButtonPressed;
		bool _isCountingToDoubleClickEnabled;	//used in detection of doubleclick
		float _timeFromLastClick;

		void _stopReplacing();
	};
}
