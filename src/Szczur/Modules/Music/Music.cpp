#include "Music.hpp"

#include <nlohmann/json.hpp>

namespace rat 
{

	Music::Music()
	{
		initScript();
		LOG_INFO(this, " : Module Music constructed");
	}

	Music::~Music()
	{
		LOG_INFO(this, " : Module Music destructed");
	}

	void Music::initScript() 
	{
		Script& script = getModule<Script>();
		auto module = script.newModule("Music");

		module.set_function("play", &Music::play, this);
		module.set_function("pause", &Music::pause, this);
		module.set_function("stop", &Music::stop, this);
		module.set_function("includes", &Music::includes, this);
		module.set_function("cleanEffects", &Music::cleanEffects, this);
		module.set_function("setPlayingMode", &Music::setPlayingMode, this);
		module.set_function("setVolume", &Music::setVolume, this);
		module.set_function("getVolume", &Music::getVolume, this);
		module.set_function("get", &Music::get, this);
		module.set_function("getCurrentPlaying", &Music::getCurrentPlaying, this);
		module.set_function("setGlobalEffects", &Music::setGlobalEffects, this);
		module.set_function("loadPlaylistFromJson", &Music::loadPlaylistFromJson, this);

		module.set_function("addPlaylist",
			[owner = this](const std::string& key, sol::variadic_args newPlaylist){
				owner->_playlists[fnv1a_32(key.c_str())] = std::make_unique<Playlist>(owner->getModule<AudioEffects>());
				for (auto it : newPlaylist){
					owner->addToPlaylist(key, it);
				}
			}
		);

		module.set_function("addToPlaylist",
			[owner = this](const std::string& key, sol::variadic_args musics){
				for (auto it : musics){
					owner->addToPlaylist(key, it);
				}
			}
		);

		module.set_function("removeFromPlaylist",
			[owner = this](const std::string& key, sol::variadic_args musics){
				for (auto it : musics){
					owner->removeFromPlaylist(key, it);
				}
			}
		);

		module.set_function("getEqualizer", &Music::getEffect<Equalizer>, this);
		module.set_function("getReverb", &Music::getEffect<Reverb>, this);
		module.set_function("getEcho", &Music::getEffect<Echo>, this);

		module.set_function("cleanEqualizer", &Music::cleanEffect<Equalizer>, this);
		module.set_function("cleanReverb", &Music::cleanEffect<Reverb>, this);
		module.set_function("cleanEcho", &Music::cleanEffect<Echo>, this);

		module.set("Random", PlayingMode::Random);
		module.set("Orderly", PlayingMode::Orderly);
		module.set("Single", PlayingMode::Single);
	}

	MusicAssets* Music::getAssetsManager()
	{
		return &_assets;
	}

	const Playlist::Container_t& Music::getPlaylist(const std::string& key)
	{
		return _playlists[fnv1a_32(key.c_str())]->getContainerRef();
	};

	void Music::loadPlaylistFromJson(const std::string& filePath)
	{
		nlohmann::json j;

		std::ifstream file(filePath);
        if (file.is_open()) {
            file >> j;
        }
        file.close();

        for (auto it = j.begin(); it != j.end(); ++it)
            addPlaylist(it.key(), it.value());
	}

	void Music::update(float deltaTime)
	{
		for(auto& it : _currentPlaylistKeys) {
			if (it != 0)
				_playlists[it]->update(deltaTime);
		}
	}

	void Music::addPlaylist(const std::string& key, const std::vector<std::string>& newPlaylist) 
	{
		_playlists[fnv1a_32(key.c_str())] = std::make_unique<Playlist>(getModule<AudioEffects>());

		for (auto it : newPlaylist)
			addToPlaylist(key, it);
	}

	void Music::addToPlaylist(const std::string& key, const std::string& name)
	{
		_assets.load(name);
		auto&& base = MusicBase(_assets.get(name));
		_playlists[fnv1a_32(key.c_str())]->add(std::move(base));

		LOG_INFO("Added ", name, " to playlist ", key);
	}

	void Music::removeFromPlaylist(const std::string& key, const std::string& name) 
	{
		auto hashKey = fnv1a_32(key.c_str()); 

		for(auto& it : _currentPlaylistKeys) {
			if (it == hashKey) 
				it = 0;
		}

		if (name.empty()) {

			for (auto& it : _playlists[hashKey]->getContainerRef())
				unLoad(it->getName());

			_playlists.erase(hashKey);
		}
		else {
			_playlists[hashKey]->remove(name);
			unLoad(name);
		}
	}

	void Music::play(unsigned int musicTrack, const std::string& key, const std::string& name)
	{
		auto hashKey      = fnv1a_32(key.c_str());
		auto samePlaylist = (_currentPlaylistKeys[musicTrack] == hashKey);

		for (unsigned int i = 0; i <= 3; ++i) {
			if (_currentPlaylistKeys[i] == hashKey && musicTrack != i) {
				LOG_INFO("Can't play the same playlist on several tracks at the same time!");
				return; 
			}
		}

		if (_currentPlaylistKeys[musicTrack] == 0 || (name == "" && samePlaylist))
			_playlists[hashKey]->play(name);
		else {
			auto currentPlaying = _playlists[_currentPlaylistKeys[musicTrack]]->getCurrentPlaying();

			if (currentPlaying->getName() != name || (_playlists[_currentPlaylistKeys[musicTrack]]->getStatus() == Playlist::Status::Stopped && samePlaylist))
				_playlists[hashKey]->play(currentPlaying, name);
			else if (!samePlaylist)
				_playlists[hashKey]->play(_playlists[hashKey]->getID(name), currentPlaying->getTimeLeft());
			
			if (!samePlaylist)
				_playlists[_currentPlaylistKeys[musicTrack]]->stopUpdates();
		}
		_currentPlaylistKeys[musicTrack] = hashKey;
	}

	void Music::pause(int musicTrack)
	{
		if (musicTrack == -1) {
			for(auto& it : _currentPlaylistKeys) {
				if (it != 0)
					_playlists[it]->pause();
			}
		}
		else if (musicTrack <= 3 && musicTrack >= 0 && _currentPlaylistKeys[musicTrack] != 0) {
			_playlists[_currentPlaylistKeys[musicTrack]]->pause();
		}
	}

	void Music::stop(int musicTrack)
	{
		if (musicTrack == -1) {
			for(auto& it : _currentPlaylistKeys) {
				if (it != 0)
					_playlists[it]->stop();
			}
		}
		else if (musicTrack <= 3 && musicTrack >= 0 && _currentPlaylistKeys[musicTrack] != 0) {
			_playlists[_currentPlaylistKeys[musicTrack]]->stop();
		}
	}

	RatMusic* Music::getCurrentPlaying(unsigned int musicTrack)
	{
		if (musicTrack <= 3 && _currentPlaylistKeys[musicTrack] != 0) {
			return &(_playlists[_currentPlaylistKeys[musicTrack]]->getCurrentPlaying()->getSource());
		}
		return nullptr;
	}

	RatMusic& Music::get(const std::string& name) 
	{
		return _assets.get(name);
	}

	bool Music::includes(const std::string& key, const std::string& name)
	{
		return _playlists[fnv1a_32(key.c_str())]->includes(name);
	}

	void Music::setPlayingMode(const std::string& key, PlayingMode mode)
	{
		switch(mode) {
			case PlayingMode::Orderly: LOG_INFO("Playing Mode in playlist ", key, " changed to Orderly"); break;
			case PlayingMode::Random: LOG_INFO("Playing Mode in playlist ", key, " changed to Random"); break;
			case PlayingMode::Single: LOG_INFO("Playing Mode in playlist ", key, " changed to Single"); break;
		}

		_playlists[fnv1a_32(key.c_str())]->setPlayingMode(mode);
	}

	void Music::setVolume(float volume, const std::string& key, const std::string& name)
	{
		if (key.empty()) {
			RatMusic::SetMusicVolume(volume);
			for (auto& it : _playlists) {
				auto& playlist = it.second->getContainerRef();
				for (auto& music : playlist) {
					music->setVolume(music->getVolume());
				}
			}
		}
		else
			_playlists[fnv1a_32(key.c_str())]->setVolume(volume, name);
	}

	float Music::getVolume(const std::string& name)
	{
		if (name.empty())
			return RatMusic::GetMusicVolume(); 
			
		return _assets.get(name).getVolume();
	}

	void Music::unLoad(const std::string& name)
	{
		auto& music = _assets.get(name);
		music.decrementCounter();

		if (!music.getCounterValue())
			_assets.unload(name);
	}

	void Music::setGlobalEffects()
	{
		for(auto& it : _currentPlaylistKeys) {
			if (it != 0)
				_playlists[it]->setGlobalEffects();
		}
	}

	void Music::cleanEffects()
	{
		for(auto i = 0; i <= 3; ++i) {
			if (_currentPlaylistKeys[i] != 0)
				getCurrentPlaying(i)->cleanAllEffects();
		}
	}

}
