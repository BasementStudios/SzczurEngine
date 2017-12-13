#include "MusicMenager.hpp"

#include <iostream>

namespace rat {

	void MusicMenager::init() 
	{

	}

	void MusicMenager::input(const sf::Event& event) 
	{

	}

	void MusicMenager::render() 
	{

	}

	void MusicMenager::update(float deltaTime) 
	{
		for(auto& it : _playlists) {
			it->update(deltaTime);
		}
	}


	void MusicMenager::push(std::vector<std::string> newPlaylist, int pos) 
	{
		if(pos < 0 || static_cast<unsigned>(pos) >= _playlists.size())
			_playlists.push_back(std::make_shared<Playlist>(newPlaylist));
		else
			_playlists[pos] = std::make_shared<Playlist>(newPlaylist);
	}

	void MusicMenager::remove(int id) 
	{
		_playlists.erase(_playlists.begin() + id);
	}

	std::shared_ptr<Playlist> MusicMenager::operator[](int id) 
	{
		return _playlists[id];
	}
	
}