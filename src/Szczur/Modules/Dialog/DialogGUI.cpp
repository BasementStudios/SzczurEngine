#include "DialogGUI.hpp"

#include "Szczur/Modules/GUI/TextAreaWidget.hpp"
#include "Szczur/Modules/GUI/TextWidget.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/ScrollAreaWidget.hpp"
#include "Szczur/Modules/GUI/Widget.hpp"

namespace rat {

    DialogGUI::DialogGUI(GUI& gui) {
        
        
    }

    DialogGUI::~DialogGUI() {
        //@TODO: remove _interface from _gui
    }

	void DialogGUI::init() {
		/*if(_onInit && _reset && 
		   _clearButtons && _getButton && 
		   _onUpdate && _interface && 
		   _dialogText && _listHolder && _list)*/
		_onInit(this);
		_reset(this);
	}

    void DialogGUI::hide() {
		_interface->fullyDeactivate();
    }

    void DialogGUI::show() {
		_interface->fullyActivate();
    }

    void DialogGUI::initScript(Script& script) {
        auto object = script.newClass<DialogGUI>("DialogGUI", "Dialog");

		object.set("onInit", &DialogGUI::_onInit);
		object.set("reset", &DialogGUI::_reset);
		object.set("clearButtons", &DialogGUI::_clearButtons);
		object.set("getButton", &DialogGUI::_getButton);

		object.set("interface", &DialogGUI::_interface);
		object.set("dialogText", &DialogGUI::_dialogText);
		object.set("listHolder", &DialogGUI::_listHolder);
		object.set("list", &DialogGUI::_list); 

        object.init();
    }

    void DialogGUI::clear() {
		_clearButtons(this);
    }

	void DialogGUI::setText(const std::string& text) {
		_dialogText->fullyActivate();
		_dialogText->setString(text);
	}

    void DialogGUI::interpretOptions(TextManager& textManager, Options& options, std::function<void(size_t, size_t, bool)> callback) {
        size_t i = 0u;
		_dialogText->fullyDeactivate();
        bool skipped{false};
        options.forEach([&i, this, callback, &textManager, &skipped](Options::Option* option){
            if(!skipped) {
                if(option->condition.valid() ? (bool)(option->condition()) : true){
                    if(option->skip) {
                        skipped = true;
                        std::invoke(callback, option->majorTarget, option->minorTarget, option->finishing);
                        if(option->afterAction.valid())
                            option->afterAction();
                    }
					auto* button = _getButton(this,
											  textManager.getLabel(option->majorTarget, option->minorTarget),
											  option->color.r,
											  option->color.g,
											  option->color.b,
											  option->color.a,
											  option->iconId
									).get<TextWidget*>();
                    button->setCallback(Widget::CallbackType::onRelease, [this, option, callback](Widget*){
                            if(option->afterAction.valid())
                                option->afterAction();
                                //std::invoke(option->afterAction);
                            std::invoke(callback, option->majorTarget, option->minorTarget, option->finishing);
                    });
                    ++i;
                }
            }
        });
    }
}
