#include "Music.hpp"


namespace rat 
{

	void Music::update(float deltaTime) 
	{
		if(_currentPlaylistID != -1)
			_playlists[_currentPlaylistID]->update(deltaTime);
	}

	void Music::addPlaylist(const std::vector<std::string>& newPlaylist) 
	{
		_playlists.push_back(std::make_unique<Playlist>());

		for (auto it : newPlaylist) {
			auto& source = getModule<Assets>().load<sf::Music>(getPath(it));
			auto&& base = MusicBase(it, source);
			_playlists.back()->add(std::move(base));
		}
	}

	void Music::remove(unsigned int id, const std::string& fileName) 
	{
		if(id > _playlists.size()) return;

		if (fileName.empty()) {
			if (id == static_cast<unsigned>(_currentPlaylistID)) 
				_currentPlaylistID = -1;

			for (auto& it : _playlists[id]->getContainerRef())
				unLoad(id, it->getName());

			_playlists.erase(_playlists.begin() + id);
		}
		else
			unLoad(id, fileName);
	}

	void Music::play(unsigned int id, const std::string& fileName)
	{
		if (_currentPlaylistID == -1) 
			_playlists[id]->play(fileName);
		else {
			auto currentPlaying = _playlists[_currentPlaylistID]->getCurrentPlaying();
			auto samePlaylist = (id == static_cast<unsigned>(_currentPlaylistID));

			if(currentPlaying->getName() != fileName)
				_playlists[id]->play(currentPlaying, fileName);
			else if(!samePlaylist)
				_playlists[id]->play(_playlists[id]->getID(fileName), currentPlaying->getTimeLeft());

			if (!samePlaylist)
				_playlists[_currentPlaylistID]->stopUpdates();
		}
		_currentPlaylistID = id;
	}

	Playlist& Music::operator[](unsigned int id) 
	{
		return *(_playlists[id].get());
	}

	inline std::string Music::getPath(const std::string& fileName) const 
	{
		return "res/Music/" + fileName + ".flac"; 
	}

	bool Music::isUsingByOtherPlaylist(unsigned int id, const std::string& fileName) const
	{
		for (unsigned int i = 0; i < _playlists.size(); ++i) {
			if (i != id) {
				if (_playlists[i]->includes(fileName))
					return true;
			}
		}
		
		return false;
	}

	void Music::unLoad(unsigned int id, const std::string& fileName)
	{
		if (!isUsingByOtherPlaylist(id, fileName))
			getModule<Assets>().unload<sf::Music>(getPath(fileName));
	}
	
}