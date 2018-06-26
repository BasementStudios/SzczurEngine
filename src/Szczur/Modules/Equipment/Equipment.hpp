#pragma once
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/Script/Script.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Utility/Logger.hpp"
#include "Szczur/Modules/GUI/Widget.hpp"

#include <vector>
namespace rat
{
	class WindowWidget; class NormalSlots; class ArmorSlots; class EquipmentObject; class ImageWidget; class ItemPreview;
	class Equipment : public Module<Window, Input, GUI> {
	public:

		void init();
		void update(float deltaTime = (1.f / 60.f));
		void render();

		Equipment();

		void enableItemPreview(EquipmentObject* item);
		void disableItemPreview();
	private:
		NormalSlots* _normalSlots;
		ArmorSlots* _armorSlots;
		ItemPreview* _itemPreview;

		Window& mainWindow;
		sf3d::RenderWindow& window;

		Widget* _base{ nullptr };
		WindowWidget* _equipmentFrame{ nullptr };

		sf::RenderTexture _canvas;

		bool isPreviewOn;
	};
}