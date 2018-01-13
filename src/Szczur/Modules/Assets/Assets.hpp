#pragma once

#include "Szczur/Utility/Module.hpp"
#include "AssetsManager.hpp"

namespace rat
{

class Assets;

template <typename T>
class _Assets : public Module<Assets, T>
{
public:

	using Manager_t = AssetsManager<sf::Font, sf::Texture, sf::Shader, sf::Music, sf::SoundBuffer>;

	_Assets()
	{
		LOG_INFO("Module Assets created");
	}

	_Assets(const _Assets&) = delete;

	_Assets& operator = (const _Assets&) = delete;

	_Assets(_Assets&&) = delete;

	_Assets& operator = (_Assets&&) = delete;

	~_Assets()
	{
		LOG_INFO("Module Assets destructed");
	}

	template <typename U>
	U& load(const std::string& path)
	{
		return _manager.load<U>(path);
	}

	template <typename U>
	bool unload(const std::string& path)
	{
		return _manager.unload<U>(path);
	}

	template <typename U>
	bool isLoaded(const std::string& path) const
	{
		return _manager.isLoaded<U>(path);
	}

	template <typename U>
	U* getPtr(const std::string& path)
	{
		return _manager.getPtr<U>(path);
	}

	template <typename U>
	const U* getPtr(const std::string& path) const
	{
		return _manager.getPtr<U>(path);
	}

	template <typename U>
	U& getRef(const std::string& path)
	{
		return _manager.getRef<U>(path);
	}

	template <typename U>
	const U& getRef(const std::string& path) const
	{
		return _manager.getRef<U>(path);
	}

private:

	Manager_t _manager;

};

REGISTER_GAME_MODULE(Assets, _Assets);

}
