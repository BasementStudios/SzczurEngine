#include "SpriteDisplayDataManager.hpp"

#include "../Scene.hpp"

#include "../ScenesManager.hpp"

namespace rat {
    SpriteDisplayDataManager::SpriteDisplayDataManager(ScenesManager& scenes) :
    _scenes{scenes} {

    }

    void SpriteDisplayDataManager::render(bool& ifRender) {
        static char enteredText[255];
        if(ImGui::Begin("Sprite Display Data Manager", &ifRender)) {
            auto& spriteDisplayDataHolder = _scenes.getCurrentScene()->getSpriteDisplayDataHolder();
            ImGui::InputText("", enteredText, 255);
            
            ImGui::SameLine();
            if(ImGui::Button("Add")) {
                try {
                    spriteDisplayDataHolder.emplace_back(enteredText);
                }
                catch (const std::exception& exc) {
                    LOG_EXCEPTION(exc);
                }
                for(int i = 0; i<255; ++i)
                    enteredText[i] = '\0';
            }
            ImGui::Separator();
            if(ImGui::BeginChild("Datas")) {
                for(auto it = spriteDisplayDataHolder.begin(); it!=spriteDisplayDataHolder.end(); ++it) {
                    if(ImGui::SmallButton("-")) {
                        spriteDisplayDataHolder.erase(it);
                        --it;
                        continue;
                    }
                    ImGui::SameLine();
                    ImGui::Text(it->getName().c_str());
                }
            }
            ImGui::EndChild();
        }
        ImGui::End();
    }
}