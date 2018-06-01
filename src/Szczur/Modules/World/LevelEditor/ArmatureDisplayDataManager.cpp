#include "ArmatureDisplayDataManager.hpp"

#include "../ScenesManager.hpp"

#include "Szczur/Utility/Convert/Windows1250.hpp"

namespace rat {
    ArmatureDisplayDataManager::ArmatureDisplayDataManager(ScenesManager& scenes) :
    _scenes{scenes} {

    }

    void ArmatureDisplayDataManager::render(bool& ifRender) {
        static char enteredText[255];
        if(ImGui::Begin("Armature Display Data Manager", &ifRender)) {
            auto& armatureDisplayDataHolder = _scenes.getArmatureDisplayDataHolder();
            if(ImGui::InputText("", enteredText, 255)) {
            }
            ImGui::SameLine();
            if(ImGui::Button("Add")) {
                try{
                    armatureDisplayDataHolder.emplace_back(enteredText);
                }
                catch (const std::exception& exc)
                {
                    LOG_EXCEPTION(exc);
                }
                for(int i = 0; i<255; ++i)
                    enteredText[i] = '\0';
            }
            ImGui::Separator();
            if(ImGui::BeginChild("Datas")) {
                for(auto it = armatureDisplayDataHolder.begin(); it!=armatureDisplayDataHolder.end(); ++it) {
                    if(ImGui::SmallButton("-")) {
                        armatureDisplayDataHolder.erase(it);
                        //--it;

                        break;
                    }
                    ImGui::SameLine();
                    ImGui::Text(mapWindows1250ToUtf8(it->getName()).c_str());
                }
            }
            ImGui::EndChild();
        }
        ImGui::End();
    }
}