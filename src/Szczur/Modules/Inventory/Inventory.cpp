#include "Inventory.hpp"

namespace rat {
    Inventory::Inventory() {
        LOG_INFO(this, "Module Inventory constructed")
        initScript();

        auto& gui = getModule<GUI>();
        

    }

    Inventory::~Inventory() {
        LOG_INFO(this, "Module Inventory destructed")
    }

    void Inventory::initScript() {
        Script& script = getModule<Script>();
        auto module = script.newModule("Inventory");
        module.set_function("set", &Inventory::set, this);
        module.set_function("addRing", &Inventory::addRing, this);
        script.initClasses<Item>();
        script.scriptFile("data/_Inventory.lua");
    }

    void Inventory::addSword(const std::string& path) {
        //getModule<Script>().get().set("THIS", _sword);
        //getModule<Script>().scriptFile(path);
        //getModule<Script>().get().set("THIS", sol::nil);
    }

    void Inventory::addArmor(const std::string& path) {

    }

    void Inventory::addRing(const std::string& path) {
        for(int i{0}; i<6; ++i) {
            if(_rings[i].isEmpty()) {
                _rings[i].setItem(new Item(_rings[i].getWidget()));
                getModule<Script>().get().set("THIS", _rings[i].getItem());
                getModule<Script>().scriptFile(path);
                getModule<Script>().get().set("THIS", sol::nil);
                break;
            }
        }

        
    }

    void Inventory::addAmulet(const std::string& path) {

    }

    void Inventory::addPotion(const std::string& path) {

    }

    void Inventory::addStone(const std::string& path) {

    }

    void Inventory::addBomb(const std::string& path) {

    }

    void Inventory::addScroll(const std::string& path) {

    }


    void Inventory::set(
        Widget* interface,
        ImageWidget* sword,
        ImageWidget* armor,
        sol::table rings,
        sol::table amulets,
        sol::table potions,
        sol::table stones,
        sol::table bombs,
        sol::table scrolls
    ) {
        _interface = interface;
        _interface->move({10.f,10.f});
        _sword.setWidget(sword);
        _armor.setWidget(armor);
        for(int i{0}; i<6; ++i)
            _rings[i].setWidget(rings[i+1]);

        for(int i{0}; i<3; ++i)
            _amulets[i].setWidget(amulets[i+1]);

        for(int i{0}; i<5; ++i)
            _potions[i].setWidget(potions[i+1]);

        for(int i{0}; i<5; ++i)
            _stones[i].setWidget(stones[i+1]);

        for(int i{0}; i<5; ++i)
            _bombs[i].setWidget(bombs[i+1]);

        for(int i{0}; i<5; ++i)
            _scrolls[i].setWidget(scrolls[i+1]);
    }

}