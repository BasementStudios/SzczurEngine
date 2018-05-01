#include "DialogData.hpp"

#include <sstream>
#include <regex>

namespace rat 
{
    void DialogData::interpretText(const std::string& text, std::vector<CharacterData>& characters)
    {
        majorFullName = "[" + std::to_string(id) + "] " + majorName;
        minorFullName = "[" + std::to_string(minorId) + "] " + minorName;

        using namespace std::string_literals;
        std::stringstream stream;
        stream << text;
        std::string temp;
        std::regex word_regex( R"([\s]*\[(\d+)\:(\d+)\][\s]*\[(.+)\](.*))"s);
        while(std::getline(stream, temp)) {
            for(auto it = std::sregex_iterator(temp.begin(), temp.end(), word_regex); it!=std::sregex_iterator(); ++it, ++dialogLines) {
                dialogTime.push_back(it->str(1) + ":" + it->str(2));
                if (auto charactersIT = std::find_if(characters.begin(), characters.end(), [&](const auto& i){ return i.name == it->str(3); }); charactersIT != characters.end()) {
                    chosenCharacter.push_back(std::distance(characters.begin(), charactersIT));
                }
                else {
                    chosenCharacter.push_back(-1);
                }
                dialogs.push_back(it->str(4) == "_" ? "" : it->str(4));
            }
        }
    }

    void DialogData::renameMajor(const std::string& name)
    {
        majorName = name;
        majorFullName = "[" + std::to_string(id) + "] " + majorName;
    }
    
    void DialogData::renameMinor(const std::string& name)
    {
        minorName = name;
        minorFullName = "[" + std::to_string(minorId) + "] " + minorName;
    }
}