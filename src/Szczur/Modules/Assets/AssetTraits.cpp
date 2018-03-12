#include "AssetTraits.hpp"

#include <fstream>
#include <streambuf>

#include <SFML/System/MemoryInputStream.hpp>

namespace rat
{

#include "Fallbacks/Font.bin"
#include "Fallbacks/Texture.bin"
#include "Fallbacks/Music.bin"
#include "Fallbacks/SoundBuffer.bin"

sf::Font* AssetTraits<sf::Font>::create()
{
	return new sf::Font;
}

sf::Font* AssetTraits<sf::Font>::createFallback()
{
	sf::Font* tmp = new sf::Font;
	tmp->loadFromMemory(fontData, sizeof(fontData));
	return tmp;
}

bool AssetTraits<sf::Font>::load(sf::Font& font, const std::string& path)
{
	return font.loadFromFile(path);
}

void AssetTraits<sf::Font>::unload(sf::Font&)
{
	// Nothing to do here, destructor will take care of freeing resource
}

const char* AssetTraits<sf::Font>::getName()
{
	return "sf::Font";
}

sf::Texture* AssetTraits<sf::Texture>::create()
{
	return new sf::Texture;
}

sf::Texture* AssetTraits<sf::Texture>::createFallback()
{
	sf::Texture* tmp = new sf::Texture;
	tmp->loadFromMemory(textureData, sizeof(textureData));
	tmp->setRepeated(true);
	return tmp;
}

bool AssetTraits<sf::Texture>::load(sf::Texture& texture, const std::string& path)
{
	return texture.loadFromFile(path);
}

void AssetTraits<sf::Texture>::unload(sf::Texture&)
{
	// Nothing to do here, destructor will take care of freeing resource
}

const char* AssetTraits<sf::Texture>::getName()
{
	return "sf::Texture";
}

sf::Shader* AssetTraits<sf::Shader>::create()
{
	return new sf::Shader;
}

sf::Shader* AssetTraits<sf::Shader>::createFallback()
{
	// @todo fallback for Shaders
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
	streams[0].open(&*it1, std::distance(it1, it2));

	it1 = std::next(it2);
	it2 = std::find(it1, std::end(buffer), '$');
	streams[1].open(&*it1, std::distance(it1, it2));

	it1 = std::next(it2);
	it2 = std::find(it1, std::end(buffer), '$');
	streams[2].open(&*it1, std::distance(it1, it2));

	return shader.loadFromStream(streams[0], streams[1], streams[2]);
}

void AssetTraits<sf::Shader>::unload(sf::Shader&)
{
	// Nothing to do here, destructor will take care of freeing resource
}

const char* AssetTraits<sf::Shader>::getName()
{
	return "sf::Shader";
}

RatMusic* AssetTraits<RatMusic>::create()
{
	return new RatMusic;
}

RatMusic* AssetTraits<RatMusic>::createFallback()
{
	RatMusic* tmp = new RatMusic;
	tmp->openFromMemory(musicData, sizeof(musicData));
	tmp->setLoop(true);
	tmp->setVolume(50.0f);
	return tmp;
}

bool AssetTraits<RatMusic>::load(RatMusic& music, const std::string& path)
{
	return music.openFromFile(path);
}

void AssetTraits<RatMusic>::unload(RatMusic&)
{
	// Nothing to do here, destructor will take care of freeing resource
}

const char* AssetTraits<RatMusic>::getName()
{
	return "RatMusic";
}

sf::SoundBuffer* AssetTraits<sf::SoundBuffer>::create()
{
	return new sf::SoundBuffer;
}

sf::SoundBuffer* AssetTraits<sf::SoundBuffer>::createFallback()
{
	sf::SoundBuffer* tmp = new sf::SoundBuffer;
	tmp->loadFromMemory(soundBufferData, sizeof(soundBufferData));
	return tmp;
}

bool AssetTraits<sf::SoundBuffer>::load(sf::SoundBuffer& soundBuffer, const std::string& path)
{
	return soundBuffer.loadFromFile(path);
}

void AssetTraits<sf::SoundBuffer>::unload(sf::SoundBuffer&)
{
	// Nothing to do here, destructor will take care of freeing resource
}

const char* AssetTraits<sf::SoundBuffer>::getName()
{
	return "sf::SoundBuffer";
}

}
