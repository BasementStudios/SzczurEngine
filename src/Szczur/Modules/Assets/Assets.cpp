#include "Assets.hpp"

#include <fstream>

#include "Szczur/Json.hpp"

namespace rat
{

void Assets::init()
{

}

void Assets::input(const sf::Event& event)
{
	(void)event;
}

void Assets::update(float deltaTime)
{
	(void)deltaTime;
}

void Assets::render()
{

}

void Assets::loadFromJsonFile(const std::string& path)
{
	Json list;
	std::ifstream(path) >> list;

	for(const Json& j : list["font"]);
	for(const Json& j : list["texture"]);
	for(const Json& j : list["music"]);
	for(const Json& j : list["soundBuffer"]);

}

}
