#include "Music.hpp"

namespace rat 
{

	Music::Music(const std::string& assetsPath)
		: _assets(assetsPath)
	{
		LOG_INFO(this, " : Module Music constructed");
		initScript();
	}

	void Music::initScript() {
		Script& script = getModule<Script>();
		auto module = script.newModule("Music");
		
		SCRIPT_SET_MODULE(Music, addToPlaylist, removeFromPlaylist, play, pause, stop, includes, setPlayingMode, setVolume, getVolume);

		module.set_function("addPlaylist",
			[owner = this](const std::string& key, sol::variadic_args newPlaylist){
				owner->_playlists[fnv1a_32(key.begin())] = std::make_unique<Playlist>();
				for (auto it : newPlaylist){
					owner->addToPlaylist(key, it);
				}
			}
		);

		script.initClasses<Equalizer, Reverb, Echo>();

		module.set_function("getEqualizer", &Music::getEffect<Equalizer>, this);
		module.set_function("getReverb", &Music::getEffect<Reverb>, this);
		module.set_function("getEcho", &Music::getEffect<Echo>, this);

		module.set_function("cleanEqualizer", &Music::cleanEffect<Equalizer>, this);
		module.set_function("cleanReverb", &Music::cleanEffect<Reverb>, this);
		module.set_function("cleanEcho", &Music::cleanEffect<Echo>, this);

	}

	void Music::update(float deltaTime) 
	{
		if (_currentPlaylistKey != 0)
			_playlists[_currentPlaylistKey]->update(deltaTime);
	}

	void Music::addPlaylist(const std::string& key, const std::vector<std::string>& newPlaylist) 
	{
		_playlists[fnv1a_32(key.begin())] = std::make_unique<Playlist>();

		for (auto it : newPlaylist)
			addToPlaylist(key, it);
	}

	void Music::addToPlaylist(const std::string& key, const std::string& fileName)
	{
		_assets.load(fileName);
		auto&& base = MusicBase(fileName, _assets.get(fileName));
		_playlists[fnv1a_32(key.begin())]->add(std::move(base));

		LOG_INFO("Added ", fileName, " to playlist ", key);
	}

	void Music::removeFromPlaylist(const std::string& key, const std::string& fileName) 
	{
		auto hashKey = fnv1a_32(key.begin()); 

		if (fileName.empty()) {
			if (_currentPlaylistKey == hashKey) 
				_currentPlaylistKey = 0;

			for (auto& it : _playlists[hashKey]->getContainerRef())
				unLoad(it->getName());

			_playlists.erase(_playlists.begin() + hashKey);
		}
		else
			unLoad(fileName);
	}

	void Music::play(const std::string& key, const std::string& fileName)
	{
		auto hashKey = fnv1a_32(key.begin());

		if (_currentPlaylistKey == 0)
			_playlists[hashKey]->play(fileName);
		else {
			auto currentPlaying = _playlists[_currentPlaylistKey]->getCurrentPlaying();
			auto samePlaylist = (_currentPlaylistKey == hashKey);

			if (currentPlaying->getName() != fileName)
				_playlists[hashKey]->play(currentPlaying, fileName);
			else if (!samePlaylist)
				_playlists[hashKey]->play(_playlists[hashKey]->getID(fileName), currentPlaying->getTimeLeft());

			if (!samePlaylist)
				_playlists[_currentPlaylistKey]->stopUpdates();
		}
		_currentPlaylistKey = hashKey;
	}

	void Music::pause()
	{
		_playlists[_currentPlaylistKey]->pause();
	}

	void Music::stop()
	{
		_playlists[_currentPlaylistKey]->stop();
	}

	bool Music::includes(const std::string& key, const std::string& fileName)
	{
		return _playlists[fnv1a_32(key.begin())]->includes(fileName);
	}

	void Music::setPlayingMode(const std::string& key, PlayingMode mode)
	{
		switch(mode) {
			case PlayingMode::Orderly: LOG_INFO("Playing Mode in playlist ", key, " changed to Orderly"); break;
			case PlayingMode::Random: LOG_INFO("Playing Mode in playlist ", key, " changed to Random"); break;
			case PlayingMode::Single: LOG_INFO("Playing Mode in playlist ", key, " changed to Single"); break;
		}

		_playlists[fnv1a_32(key.begin())]->setPlayingMode(mode);
	}

	void Music::setVolume(const std::string& key, float volume, const std::string& fileName)
	{
		_playlists[fnv1a_32(key.begin())]->setVolume(volume, fileName);
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