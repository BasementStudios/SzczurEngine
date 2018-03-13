#include "Music.hpp"

namespace rat 
{

	void Music::update(float deltaTime) 
	{
		if (_currentPlaylistID != -1)
			_playlists[_currentPlaylistID]->update(deltaTime);
	}

	void Music::addPlaylist(const std::vector<std::string>& newPlaylist) 
	{
		_playlists.push_back(std::make_unique<Playlist>());

		for (auto it : newPlaylist)
			addToPlaylist(_playlists.size() - 1, it);
	}

	void Music::addToPlaylist(unsigned int id, const std::string& fileName)
	{
		_assets.load(fileName);
		auto&& base = MusicBase(fileName, _assets.get(fileName));
		_playlists[id]->add(std::move(base));

		LOG_INFO("Added ", fileName, " to playlist nr:", id);
	}

	void Music::removeFromPlaylist(unsigned int id, const std::string& fileName) 
	{
		if (id > _playlists.size()) return;

		if (fileName.empty()) {
			if (id == static_cast<unsigned>(_currentPlaylistID)) 
				_currentPlaylistID = -1;

			for (auto& it : _playlists[id]->getContainerRef())
				unLoad(it->getName());

			_playlists.erase(_playlists.begin() + id);
		}
		else
			unLoad(fileName);
	}

	void Music::play(unsigned int id, const std::string& fileName)
	{
		LOG_INFO("Play function started: ", fileName, " in playlist nr:", id);

		if (_currentPlaylistID == -1) 
			_playlists[id]->play(fileName);
		else {
			auto currentPlaying = _playlists[_currentPlaylistID]->getCurrentPlaying();
			auto samePlaylist = (id == static_cast<unsigned>(_currentPlaylistID));

			if (currentPlaying->getName() != fileName)
				_playlists[id]->play(currentPlaying, fileName);
			else if (!samePlaylist)
				_playlists[id]->play(_playlists[id]->getID(fileName), currentPlaying->getTimeLeft());

			if (!samePlaylist)
				_playlists[_currentPlaylistID]->stopUpdates();
		}
		_currentPlaylistID = id;
	}

	void Music::pause()
	{
		_playlists[_currentPlaylistID]->pause();
	}

	void Music::stop()
	{
		_playlists[_currentPlaylistID]->stop();
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

	float Music::getVolume(const std::string& fileName)
	{
		return _assets.get(fileName).getVolume();
	}

	void Music::unLoad(const std::string& fileName)
	{
		auto& music = _assets.get(fileName);
		music.decrementCounter();

		if (!music.getCounterValue())
			_assets.unload(fileName);
	}	

}