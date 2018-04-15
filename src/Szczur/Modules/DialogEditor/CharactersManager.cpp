#include "CharactersManager.hpp"

#include <fstream>

#include <nlohmann/json.hpp>

#include "Szczur/Config.hpp"

namespace rat
{
    CharactersManager::CharactersManager()
    {

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

    void CharactersManager::load(const std::string& path)
    {
        using Json = nlohmann::json;
        Json json;

        std::ifstream file(path);
        if(file.is_open()) {
            file >> json;
        }
        file.close();

        for (auto it = json.begin(); it != json.end(); ++it)
        {
            _charactersName.push_back(it.key());
            _charactersImagePath.push_back(it.value() == nullptr ? "" : it.value());
        }

    }

    void CharactersManager::save(const std::string& path)
    {
        using Json = nlohmann::json;
        Json json;

        for(unsigned int i = 0; i < _charactersName.size(); ++i) {
            json[_charactersName[i]] = _charactersImagePath[i];
        }

        std::ofstream file(path, std::ios::trunc);
        if(file.is_open()) {
            file << json;
        }
        file.close();
    }
    
}