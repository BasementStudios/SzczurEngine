#include "MusicManager.hpp"

#include <iostream>

namespace rat 
{

	void MusicManager::init() 
	{

	}

	void MusicManager::input(const sf::Event& event) 
	{

	}

	void MusicManager::render() 
	{

	}

	void MusicManager::update(float deltaTime) 
	{
		for (auto& it : _playlists)
			it->update(deltaTime);
	}


	void MusicManager::push(std::vector<std::string> newPlaylist, int pos) 
	{
		if (pos < 0 || static_cast<unsigned>(pos) >= _playlists.size())
			_playlists.push_back(std::make_shared<Playlist>(newPlaylist));
		else
			_playlists[pos] = std::make_shared<Playlist>(newPlaylist);
	}

	void MusicManager::remove(int id) 
	{
		_playlists.erase(_playlists.begin() + id);
	}

	std::shared_ptr<Playlist> MusicManager::operator[](int id) 
	{
		return _playlists[id];
	}
	
}