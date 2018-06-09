#pragma once
#include "Szczur/Utility/Modules/Module.hpp"
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/Sound/SoundManager.hpp"

#include <ImGui/imgui.h>
#include <ImGui/imgui-SFML.h>
#include <fstream>
namespace rat
{
    class SoundEditor : public Module <Window, Input, SoundManager>
    {
        SoundManager& _soundManager;
        struct SoundInfo{
            int id;
            float volume;
            float pitch;
            float offsetTime[2];
            bool enable;
            bool toDelete;
            std::string name;
            std::string fileName;
        };
        std::vector<std::unique_ptr<SoundInfo>> _soundsInfo;

        bool deleteField=false;
        float mini=0;
        float max=100;
        char file[256]={};
    public:

        SoundEditor();

        void init();
        void render();

        bool addSound(const std::string& soundName,const std::string& soundFileName);
        void clear();

        void save(const std::string& fileName);
        void load(const std::string& fileName);
    
    };
}