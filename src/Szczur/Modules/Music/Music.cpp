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

		for (auto it : newPlaylist)
			add(_playlists.size() - 1, it);
	}

	void Music::add(unsigned int id, const std::string& fileName)
	{
		auto& source = getModule<Assets>().load<sf::Music>(getPath(fileName));
		auto&& base = MusicBase(fileName, source);
		_playlists[id]->add(std::move(base));
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

	void Music::pause(unsigned int id)
	{
		_playlists[id]->pause();
	}

	void Music::stop(unsigned int id)
	{
		_playlists[id]->stop();
	}

	bool Music::includes(unsigned int id, const std::string& fileName) const
	{
		return _playlists[id]->includes(fileName);
	}

	void Music::setPlayingMode(unsigned int id, PlayingMode mode)
	{
		_playlists[id]->setPlayingMode(mode);
	}

	void Music::setVolume(unsigned int id, float volume, const std::string& fileName)
	{
		_playlists[id]->setVolume(volume, fileName);
	}

	float Music::getVolume(unsigned int id, const std::string& fileName) const
	{
		return _playlists[id]->getVolume(fileName);
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