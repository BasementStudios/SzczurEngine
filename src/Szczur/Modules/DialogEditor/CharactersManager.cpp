#include "CharactersManager.hpp"

#include "Szczur/Config.hpp"

namespace rat
{
    CharactersManager::CharactersManager()
    {
        //[WIP]
        _charactersName.push_back("Mroczny");
        _charactersImagePath.push_back("");
        _charactersName.push_back("Locha");
        _charactersImagePath.push_back("");

        load();
    }

    void CharactersManager::update()
    {
        ImGui::Begin("Characters Manager");
            for(unsigned int i = 0; i < _charactersName.size(); ++i) {
                auto name =  _charactersName.begin() + i;
                auto imagePath = _charactersImagePath.begin() + i;

                ImGui::PushID(i);

                std::string clipText = (ImGui::GetClipboardText() != nullptr) ? ImGui::GetClipboardText() : "";
                size_t clipSize = clipText.length();  
                size_t size = name->length() + clipSize + 30;
                char *newText = new char[size] {};
                strncpy(newText, name->c_str(), size);

                ImGui::PushItemWidth(ImGui::GetWindowWidth() - 100);
                ImGui::InputText("##Characters Name Input", newText, size);
                ImGui::PopItemWidth();

                *name = newText;
                delete[] newText;

                ImGui::SameLine();
                if (ImGui::Button("Load##Characters Manager")) {
                    //TODO: loading image for character
                }

                ImGui::SameLine(); 
                if (ImGui::Button("-##Characters Manager", ImVec2(30, 22))) { //TODO: repair bug in dlg editor
                    _charactersName.erase(name);
                    _charactersImagePath.erase(imagePath);
                }

                ImGui::PopID();
            }
            if (ImGui::Button("+##Characters Manager", ImVec2(30, 27))) {
                _charactersName.push_back("");
                _charactersImagePath.push_back("");
            };
        ImGui::End();
    }

    std::vector<std::string>& CharactersManager::getNamesContainer()
    {
        return _charactersName;
    }

    std::vector<std::string>& CharactersManager::getImagePathsContainer()
    {
        return _charactersImagePath;
    }

    void CharactersManager::load()
    {
        //TODO: load from json
    }

    void CharactersManager::save()
    {
        //TODO: Save to json file
    }
    
}