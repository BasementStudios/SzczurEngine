#include "AssetTraits.hpp"

/** @file AssetTraits.cpp
 ** @description File with standard assets traits implementations.
 ** @author Patryk (Stritch)
 **/

#include <fstream>
#include <streambuf>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/System/MemoryInputStream.hpp>

#include "AssetNotFoundException.hpp"

namespace rat
{

// sf::Font
sf::Font* AssetTraits<sf::Font>::create()
{
	return new sf::Font;
}
sf::Font* AssetTraits<sf::Font>::load(sf::Font& font, const std::string& path)
{
	if (!font.loadFromFile(path)) {
		throw AssetNotFoundException<sf::Font>(path);
		return nullptr;
	}
	return &font;
}
void AssetTraits<sf::Font>::unload(sf::Font& font)
{
	font.~Font();
}

// sf::Texture
sf::Texture* AssetTraits<sf::Texture>::create()
{
	return new sf::Texture;
}
sf::Texture* AssetTraits<sf::Texture>::load(sf::Texture& texture, const std::string& path)
{
	if (!texture.loadFromFile(path)) {
		throw AssetNotFoundException<sf::Texture>(path);
		return nullptr;
	}
	return &texture;
}
void AssetTraits<sf::Texture>::unload(sf::Texture& texture)
{
	texture.~Texture();
}

// sf::Shader
sf::Shader* AssetTraits<sf::Shader>::create()
{
	return new sf::Shader;
}
sf::Shader* AssetTraits<sf::Shader>::load(sf::Shader& shader, const std::string& path)
{
	std::ifstream file(path);

	if (!file.good()) {
		throw AssetNotFoundException<sf::Texture>(path);
		return nullptr;
	}

	std::string buffer;

	file.seekg(0, std::ios::end);
	buffer.reserve(file.tellg());
	file.seekg(0, std::ios::beg);

	buffer.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());

	sf::MemoryInputStream streams[3];

	auto it1 = std::begin(buffer);
	auto it2 = std::find(it1, std::end(buffer), '$');
	streams[0].open(it1.base(), std::distance(it1, it2));

	it1 = std::next(it2);
	it2 = std::find(it1, std::end(buffer), '$');
	streams[1].open(it1.base(), std::distance(it1, it2));

	it1 = std::next(it2);
	it2 = std::find(it1, std::end(buffer), '$');
	streams[2].open(it1.base(), std::distance(it1, it2));

	if (!shader.loadFromStream(streams[0], streams[1], streams[2])) {
		throw AssetNotFoundException<sf::Shader>(path + " - invaild");
		return nullptr;
	}
	return &shader;
}
void AssetTraits<sf::Shader>::unload(sf::Shader& shader)
{
	shader.~Shader();
}

// sf::Music
sf::Music* AssetTraits<sf::Music>::create()
{
	return new sf::Music;
}
sf::Music* AssetTraits<sf::Music>::load(sf::Music& music, const std::string& path)
{
	if (!music.openFromFile(path)) {
		throw AssetNotFoundException<sf::Music>(path);
		return nullptr;
	}
	return &music;
}
void AssetTraits<sf::Music>::unload(sf::Music& music)
{
	music.~Music();
}

// sf::SoundBuffer
sf::SoundBuffer* AssetTraits<sf::SoundBuffer>::create()
{
	return new sf::SoundBuffer;
}
sf::SoundBuffer* AssetTraits<sf::SoundBuffer>::load(sf::SoundBuffer& soundBuffer, const std::string& path)
{
	if (!soundBuffer.loadFromFile(path)) {
		throw AssetNotFoundException<sf::SoundBuffer>(path);
		return nullptr;
	}
	return &soundBuffer;
}
void AssetTraits<sf::SoundBuffer>::unload(sf::SoundBuffer& soundBuffer)
{
	soundBuffer.~SoundBuffer();
}

}
