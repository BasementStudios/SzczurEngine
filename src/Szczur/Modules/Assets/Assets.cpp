#include "Assets.hpp"

#include <fstream>

#include "Szczur/Json.hpp"

namespace rat
{

void Assets::init()
{

}

bool Assets::loadFromJsonFile(const std::string& path)
{
	Json list;

    std::ifstream file(path);

    if(!file.good()) return false;

	file >> list;

	for (const Json& j : list["font"]) {
        _manager.load<sf::Font>(j.get<std::string>());
    }

	for (const Json& j : list["texture"]) {
        _manager.load<sf::Texture>(j.get<std::string>());
    }

	for (const Json& j : list["shader"]) {
        _manager.load<sf::Shader>(j.get<std::string>());
    }

	for (const Json& j : list["music"]) {
        _manager.load<sf::Music>(j.get<std::string>());
    }

	for (const Json& j : list["soundBuffer"]) {
        _manager.load<sf::SoundBuffer>(j.get<std::string>());
    }

    return true;
}

}
