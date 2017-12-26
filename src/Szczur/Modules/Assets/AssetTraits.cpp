#include "AssetTraits.hpp"

#include <fstream>
#include <streambuf>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/System/MemoryInputStream.hpp>

namespace rat
{

sf::Font* AssetTraits<sf::Font>::create()
{
	return new sf::Font;
}

bool AssetTraits<sf::Font>::load(sf::Font& font, const std::string& path)
{
	return font.loadFromFile(path);
}

void AssetTraits<sf::Font>::unload(sf::Font& font)
{
	font.~Font();
}

sf::Texture* AssetTraits<sf::Texture>::create()
{
	return new sf::Texture;
}

bool AssetTraits<sf::Texture>::load(sf::Texture& texture, const std::string& path)
{
	return texture.loadFromFile(path);
}

void AssetTraits<sf::Texture>::unload(sf::Texture& texture)
{
	texture.~Texture();
}

sf::Shader* AssetTraits<sf::Shader>::create()
{
	return new sf::Shader;
}

bool AssetTraits<sf::Shader>::load(sf::Shader& shader, const std::string& path)
{
	std::ifstream file(path);

	if (!file.good())
		return false;

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

	return shader.loadFromStream(streams[0], streams[1], streams[2]);
}

void AssetTraits<sf::Shader>::unload(sf::Shader& shader)
{
	shader.~Shader();
}

sf::Music* AssetTraits<sf::Music>::create()
{
	return new sf::Music;
}

bool AssetTraits<sf::Music>::load(sf::Music& music, const std::string& path)
{
	return music.openFromFile(path);
}

void AssetTraits<sf::Music>::unload(sf::Music& music)
{
	music.~Music();
}

sf::SoundBuffer* AssetTraits<sf::SoundBuffer>::create()
{
	return new sf::SoundBuffer;
}

bool AssetTraits<sf::SoundBuffer>::load(sf::SoundBuffer& soundBuffer, const std::string& path)
{
	return soundBuffer.loadFromFile(path);
}

void AssetTraits<sf::SoundBuffer>::unload(sf::SoundBuffer& soundBuffer)
{
	soundBuffer.~SoundBuffer();
}

}
