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

	template <typename U>
	U& load(const std::string& path);

	template <typename U>
	bool unload(const std::string& path);

	template <typename U>
	bool isLoaded(const std::string& path) const;

	template <typename U>
	U* getPtr(const std::string& path);
	template <typename U>
	const U* getPtr(const std::string& path) const;

	template <typename U>
	U& getRef(const std::string& path);
	template <typename U>
	const U& getRef(const std::string& path) const;

};

}

#include "Assets.tpp"
