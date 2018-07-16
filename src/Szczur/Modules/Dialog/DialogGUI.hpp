#pragma once

#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/GUI/ListWidget.hpp"
#include "Szczur/Modules/GUI/Widget.hpp"
#include "Szczur/Modules/GUI/TextAreaWidget.hpp"
#include "Szczur/Modules/GUI/InterfaceWidget.hpp"
#include "Szczur/Modules/Script/Script.hpp"


#include "TextManager.hpp"

#include "Options.hpp"

namespace rat {
    class TextAreaWidget;
    class TextWidget;
    class ImageWidget;
    class Widget;
    class ScrollAreaWidget;
    class DialogGUI {
    public:
        DialogGUI(GUI& gui);

        ~DialogGUI();

		void init();

        void hide();

        void show();

        static void initScript(Script& script);

        void clear();

        void setText(const std::string& text);

        void interpretOptions(TextManager& textManager, Options& options, std::function<void(size_t, size_t, bool)> callback);
    private:
		sol::function _onInit;
		sol::function _reset;
		sol::function _clearButtons;
		sol::function _getButton;
		InterfaceWidget* _interface{nullptr};
		TextAreaWidget* _dialogText{nullptr};
		Widget* _listHolder{nullptr};
		ListWidget* _list{nullptr};
    };
}