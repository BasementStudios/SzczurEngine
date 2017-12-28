#pragma once

#include "Szczur/Utility/Modules.hpp"

#include "AssetsManager.hpp"

namespace rat
{

class Assets : public Module<>
{
	using Module::Module;

public:

	using Manager_t = AssetsManager<sf::Font, sf::Texture, sf::Shader, sf::Music, sf::SoundBuffer>;

private:

	Manager_t _manager;

public:

	void init();

	bool loadFromJsonFile(const std::string& path);

	template<typename TType>
	bool load(const std::string& path);

	template<typename TType>
	bool unload(const std::string& path);

	template<typename TType>
	bool isLoaded(const std::string& path) const;

	template<typename TType>
	TType* getPtr(const std::string& path);
	template<typename TType>
	const TType* getPtr(const std::string& path) const;

	template<typename TType>
	TType& get(const std::string& path);
	template<typename TType>
	const TType& get(const std::string& path) const;

};

}

#include "Assets.tpp"
