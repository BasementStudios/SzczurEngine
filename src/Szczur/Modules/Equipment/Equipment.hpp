#pragma once
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/Script/Script.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Utility/Logger.hpp"
#include "Szczur/Modules/GUI/Widget.hpp"

#include <vector>

#include "EquipmentObject.hpp"
namespace rat
{
	class WindowWidget; class NormalSlots; class ArmorSlots; class ImageWidget; class ItemPreview;
	class Equipment : public Module<Window, Input, GUI, Script> {
	public:

		void init();
		void initScript();
		void update(float deltaTime = (1.f / 60.f));
		void render();

		Equipment();

		void enableItemPreview(EquipmentObject* item);
		void disableItemPreview();

		bool canPreviewBeInstantiated;

		void createItem(std::string nameId, std::string name, std::string description, std::string iconPath, equipmentObjectType type);	//creating new item and adding it to list
		bool addItem(std::string nameId);
		bool removeItem(std::string nameId);
		sol::table getItemsList();
		int getFreeSlotsAmount();
		void resizeSlots(int newCapacity);
		int getSlotsAmount();
		void setRingsLimit(int newCapacity);


		void setCallback(std::string nameId, sol::function firstCallback);
		void setCallback(std::string nameId, sol::function firstCallback, sol::function secondCallback);

		//cos z tym jsonem

	private:
		NormalSlots* _normalSlots;
		ArmorSlots* _armorSlots;
		ItemPreview* _itemPreview;

		Window& mainWindow;
		sf3d::RenderWindow& window;

		Widget* _base{ nullptr };
		WindowWidget* _equipmentFrame{ nullptr };

		sf::RenderTexture _canvas;

		bool isPreviewOn = false;

		std::map<std::string, EquipmentObject*> _listOfObjects;
	};
}