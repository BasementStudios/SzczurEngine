#include "CharactersManager.hpp"

#include <fstream>
#include <experimental/filesystem>

#include <nlohmann/json.hpp>

#include "Szczur/Modules/FileSystem/FileDialog.hpp"
#include "Szczur/Config.hpp"

#include "Szczur/Utility/Logger.hpp"

namespace rat
{
    CharactersManager::CharactersManager(DLGEditor::TextContainer_t& dialogParts)
        : _dialogParts(dialogParts)
    {

    }

    void CharactersManager::update()
    {
        ImGui::Begin("Characters Manager");
            for(unsigned int i = 0; i < _characters.size(); ++i) {
                ImGui::PushID(i);

                auto isImageLoaded = !_characters[i].imagePath.empty();
                ImGui::RadioButton("##Is Character Loaded", isImageLoaded);
                if (ImGui::IsItemHovered()) {
                    ImGui::BeginTooltip();
                    ImGui::Text(isImageLoaded ? _characters[i].imagePath.c_str() : "The image hasn't been loaded yet.");
                    ImGui::EndTooltip();
                }

                size_t size = _characters[i].name.length() + 100;
                char *newText = new char[size] {};
                strncpy(newText, _characters[i].name.c_str(), size);
                
                ImGui::SameLine();
                ImGui::PushItemWidth(ImGui::GetWindowWidth() - 130);
                ImGui::InputText("##Characters Name Input", newText, size);
                ImGui::PopItemWidth();

                _characters[i].name = newText;
                delete[] newText;

                ImGui::SameLine();
                if (ImGui::Button("Load##Characters Manager")) {
                    auto currentPath = std::experimental::filesystem::current_path().string();
                    auto path = FileDialog::getOpenFileName("", currentPath, "Images (*.png, *.jpg)|*.png;*.jpg");
                    size_t start = path.find(currentPath);
                    if (start != -1) {
                        _characters[i].imagePath = path.substr(currentPath.length() + 1, path.length() - currentPath.length() - 1);
                    } 
                    else {
                        _characters[i].imagePath = path;
                    }
                    std::replace(_characters[i].imagePath.begin(), _characters[i].imagePath.end(), '\\', '/');
                    
                    LOG_INFO("Image path loaded: ", _characters[i].imagePath);

                }

                ImGui::SameLine(); 
                if (ImGui::Button("-##Characters Manager", ImVec2(30, 22))) {
                    LOG_INFO("Character ", _characters[i].name, "has been removed");
                    _characters.erase(_characters.begin() + i);

                    for(auto& majors : _dialogParts) {
                        for(auto& minor : majors.second) {
                            for (auto& it : minor.second->chosenCharacter) {
                                if (i < static_cast<unsigned>(it)) {
                                    --it;
                                }
                            }
                        }
                    }

                }

                ImGui::PopID();
            }
            if (ImGui::Button("+##Characters Manager", ImVec2(30, 27))) {
                _characters.push_back({"", ""});
                LOG_INFO("New Character added");
            };
        ImGui::End();
    }

    void CharactersManager::clear()
    {
        _characters.clear();
        LOG_INFO("Charactes cleared");
    }

    std::vector<CharacterData>& CharactersManager::getCharactersContainer()
    {
        return _characters;
    }

    void CharactersManager::load(const std::string& path)
    {
        clear();

        nlohmann::json json;

        std::ifstream file(path);
        if(file.is_open()) {
            file >> json;
        }
        file.close();

        for (auto it = json.begin(); it != json.end(); ++it)
        {
            _characters.push_back({it.key(), it.value()});
        }

        LOG_INFO("Charactes data loaded from: ", path);

    }

    void CharactersManager::save(const std::string& path)
    {
        nlohmann::json json;

        for(auto it : _characters) {
            json[it.name] = it.imagePath;
        }

        std::ofstream file(path, std::ios::trunc);
        if(file.is_open()) {
            file << json;
        }
        file.close();

        LOG_INFO("Charactes data saved into: ", path);
    }
    
}