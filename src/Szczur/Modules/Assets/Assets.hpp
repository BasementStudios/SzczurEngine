#pragma once

#include "AssetsManager.hpp"
#include "Szczur/Utility/Module.hpp"

namespace rat
{

class Assets : public Module<>
{
public:

	using Manager_t = AssetsManager<sf::Font, sf::Texture, sf::Shader, sf::Music, sf::SoundBuffer>;

	template <typename Tuple>
	Assets(Tuple&& tuple);

	Assets(const Assets&) = delete;

	Assets& operator = (const Assets&) = delete;

	Assets(Assets&&) = delete;

	Assets& operator = (Assets&&) = delete;

	~Assets();

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

private:

	Manager_t _manager;

};

template <typename Tuple>
Assets::Assets(Tuple&& tuple) :
	Module(tuple)
{
	LOG_INFO(this, " : Module Assets created");
}

}

#include "Assets.tpp"
