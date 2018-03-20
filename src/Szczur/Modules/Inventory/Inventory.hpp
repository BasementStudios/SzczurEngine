#pragma once

#include "Szczur/Utility/Modules/Module.hpp"
#include "Szczur/Modules/Script/Script.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"

#include "Slot.hpp"
#include "Ring.hpp"

namespace rat {
    class Inventory : public Module<Script, GUI> {
    public:
        Inventory();
        ~Inventory();

        void initScript();

        void addSword(const std::string& path);
        void addArmor(const std::string& path);
        void addRing(const std::string& path);
        void addAmulet(const std::string& path);
        void addPotion(const std::string& path);
        void addStone(const std::string& path);
        void addBomb(const std::string& path);
        void addScroll(const std::string& path);

        void set(
            Widget* interface,
            ImageWidget* sword,
            ImageWidget* armor,
            sol::table rings,
            sol::table amulets,
            sol::table potions,
            sol::table stones,
            sol::table bombs,
            sol::table scrolls
        );

    private:
        Widget* _interface;

        Slot _sword;
        Slot _armor;
        Slot _rings[6];
        Slot _amulets[3];
        Slot _potions[5];
        Slot _stones[5];
        Slot _bombs[5];
        Slot _scrolls[5];
    };
}