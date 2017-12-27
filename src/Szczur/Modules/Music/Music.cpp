#include "Music.hpp"

#include <iostream>

namespace rat 
{

	void Music::init() 
	{

	}

	void Music::input(const sf::Event& event) 
	{

	}

	void Music::render() 
	{

	}

	void Music::update(float deltaTime) 
	{
		for (auto& it : _playlists)
			it->update(deltaTime);
	}


	void Music::push(std::vector<std::string> newPlaylist, int pos) 
	{
		if (pos < 0 || static_cast<unsigned>(pos) >= _playlists.size())
			_playlists.push_back(std::make_shared<Playlist>(newPlaylist));
		else
			_playlists[pos] = std::make_shared<Playlist>(newPlaylist);
	}

	void Music::remove(int id) 
	{
		_playlists.erase(_playlists.begin() + id);
	}

	std::shared_ptr<Playlist> Music::operator[](int id) 
	{
		return _playlists[id];
	}
	
}