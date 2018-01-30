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


	void Music::push(const std::vector<std::string>& newPlaylist, int pos) 
	{
		if (pos < 0 || static_cast<unsigned>(pos) >= _playlists.size())
			_playlists.push_back(std::make_shared<Playlist>(newPlaylist));
		else
			_playlists[pos] = std::make_shared<Playlist>(newPlaylist);
	}

	void Music::remove(unsigned int id) 
	{
		_playlists.erase(_playlists.begin() + id);
	}

	void Music::play(unsigned int id, const std::string& fileName)
	{
		if (_currentPlaylistID == -1)
			_playlists[id]->play(fileName);
		else {
			_playlists[id]->play(_playlists[_currentPlaylistID]->getCurrentPlaying(), fileName);
			if (id != static_cast<unsigned>(_currentPlaylistID))
				_playlists[_currentPlaylistID]->stopUpdates();
		}
		_currentPlaylistID = id;
	}

	std::shared_ptr<Playlist> Music::operator[](unsigned int id) 
	{
		return _playlists[id];
	}
	
}