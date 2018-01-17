#pragma once

#include "Szczur/Utility/Modules.hpp"
#include "AssetsManager.hpp"

namespace rat
{

class Assets : public Module<>
{
public:

	using Manager_t = AssetsManager<sf::Font, sf::Texture, sf::Shader, sf::Music, sf::SoundBuffer>;

	template <typename Tuple>
	Assets(Tuple&& tuple) :
		Module(tuple)
	{
		LOG_INFO(this, " -> Module Assets created");
	}

	Assets(const Assets&) = delete;

	Assets& operator = (const Assets&) = delete;

	Assets(Assets&&) = delete;

	Assets& operator = (Assets&&) = delete;

	~Assets()
	{
		LOG_INFO(this, " -> Module Assets destructed");
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

}
