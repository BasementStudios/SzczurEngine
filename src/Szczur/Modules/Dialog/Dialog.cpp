#include "Dialog.hpp"

namespace rat {
    Dialog::Dialog(const std::string& file) :
    _dialogGUI(getModule<GUI>()) {
        LOG_INFO(this, "Module Dialog constructed")
        auto& gui = getModule<GUI>();
        gui.addAsset<Json>(file);
        gui.addAsset<sf::Texture>("data/dial_back.png");
        gui.addAsset<sf::Font>("data/consolab.ttf");
        
        _dialogGUI.create();
        _initScript();

        

/*        Options* opt1 = new Options;
        opt1->setRunners(1u);
        opt1->addOption(
            "Nie powinno cie tu byc",
            nullptr,
            2u,
            nullptr
        );
        opt1->addOption(
            "Znasz ich?",
            nullptr,
            3u,
            nullptr
        );

        Options* opt2 = new Options;
        opt2->setRunners(2u);
        opt2->addOption(
            "Widok ze zbocza",
            nullptr,
            1u,
            nullptr
        );
        opt2->addOption(
            "Znasz ich?",
            nullptr,
            3u,
            nullptr
        );

        Options* opt3 = new Options;
        opt3->setRunners(3u);
        opt3->addOption(
            "Widok ze zbocza",
            nullptr,
            1u,
            nullptr
        );
        opt3->addOption(
            "Nie powinno cie tu byc",
            nullptr,
            2u,
            nullptr
        );


        auto* mngr = load("data/dialog2");
        mngr->addOptions(opt1, opt2, opt3);
        mngr->startWith(1u);
        mngr->play();
        */
    }


    Dialog::~Dialog() {
        LOG_INFO(this, "Module Dialog destructed")
        for(auto& it : _dialogs)
            delete it.second;
    }

    void Dialog::_initScript() {
        Script& script = getModule<Script>();
        auto module = script.newModule("Dialog");

        SCRIPT_SET_MODULE(Dialog, load)
        ;


        script.initClasses<DialogManager, Options>();
    }



    void Dialog::update(float deltaTime) {
        for(auto& it : _dialogs)
            it.second->update(deltaTime);
        if(getModule<Input>().getManager().isReleased(Keyboard::Space))
            for(auto& it : _dialogs)
                it.second->skip();
    }

    DialogManager* Dialog::load(const std::string& path) {
        Key_t key = fnv1a_32(path.begin(), path.end());
        if(auto it = _dialogs.find(key); it == _dialogs.end() ) {
            DialogManager* result = new DialogManager(path, _dialogGUI);
            _dialogs.insert_or_assign(key, result);
            return result;
        }
        return nullptr;
    }

    bool Dialog::unload(const std::string& path) {
        if(auto it = _dialogs.find(fnv1a_32(path.begin(), path.end())); it != _dialogs.end() ) {
            delete it->second;
            _dialogs.erase(it);
            return true;
        }
        return false;
    }
}