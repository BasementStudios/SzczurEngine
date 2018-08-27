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

		module.set_function("loadMusic", &Music::loadMusic, this);
		module.set_function("removeMusic", &Music::removeMusic, this);
		module.set_function("getCurrentPlaying", &Music::getCurrentPlaying, this);

		module.set_function("play", &Music::play, this);
		module.set_function("pause", &Music::pause, this);
		module.set_function("stop", &Music::stop, this);

		module.set_function("includes", &Music::includes, this);

		module.set_function("setPlayingMode", &Music::setPlayingMode, this);

		module.set_function("setVolume", &Music::setVolume, this);
		module.set_function("getVolume", sol::overload(
			[&](int a, const std::string& b) { return getVolume(a, b); },
			[&](const std::string& a, const std::string& b) { return getVolume(a, b); },
			[&](const std::string& a) { return getVolume(a, ""); },
			[&](int a) { return getVolume(a, ""); },
			[&]() { return getVolume(-1, ""); }
		));

		module.set_function("getStatus", sol::overload(
			[&](unsigned int a, const std::string& b) { return getStatus(a, b); },
			[&](const std::string& a, const std::string& b) { return getStatus(a, b); },
			[&](unsigned int a) { return getStatus(a, ""); },
			[&](const std::string& a) { return getStatus(a, ""); }
		));

		module.set_function("loadPlaylistFromJson", &Music::loadPlaylistFromJson, this);

		module.set_function("addPlaylist",
			[owner = this](const std::string& key, sol::variadic_args newPlaylist) {
			owner->_playlists[fnv1a_32(key.c_str())] = std::make_unique<Playlist>(owner->getModule<AudioEffects>());
			for (auto it : newPlaylist) {
				owner->addToPlaylist(key, it);
			}
		}
		);
		module.set_function("addToPlaylist",
			[owner = this](const std::string& key, sol::variadic_args musics) {
			for (auto it : musics) {
				owner->addToPlaylist(key, it);
			}
		}
		);
		module.set_function("removeFromPlaylist", sol::overload(
			[&](const std::string& key) { removeFromPlaylist(key, ""); },
			[owner = this](const std::string& key, sol::variadic_args musics) {
			for (auto it : musics) {
				owner->removeFromPlaylist(key, it);
			}
		}
		));

		module.set_function("cleanEffects", &Music::cleanEffects, this);
		module.set_function("setGlobalEffects", &Music::setGlobalEffects, this);
		module.set_function("getEqualizer", sol::overload(
			[&](unsigned int a, const std::string& b) { return getEffect<Equalizer>(a, b); },
			[&](const std::string& a, const std::string& b) { return getEffect<Equalizer>(a, b); }
		));
		module.set_function("getReverb", sol::overload(
			[&](unsigned int a, const std::string& b) { return getEffect<Reverb>(a, b); },
			[&](const std::string& a, const std::string& b) { return getEffect<Reverb>(a, b); }
		));
		module.set_function("getEcho", sol::overload(
			[&](unsigned int a, const std::string& b) { return getEffect<Echo>(a, b); },
			[&](const std::string& a, const std::string& b) { return getEffect<Echo>(a, b); }
		));

		module.set_function("cleanEqualizer", sol::overload(
			[&](unsigned int a, const std::string& b) { cleanEffect<Equalizer>(a, b); },
			[&](const std::string& a, const std::string& b) { cleanEffect<Equalizer>(a, b); }
		));
		module.set_function("cleanReverb", sol::overload(
			[&](unsigned int a, const std::string& b) { cleanEffect<Reverb>(a, b); },
			[&](const std::string& a, const std::string& b) { cleanEffect<Reverb>(a, b); }
		));
		module.set_function("cleanEcho", sol::overload(
			[&](unsigned int a, const std::string& b) { cleanEffect<Echo>(a, b); },
			[&](const std::string& a, const std::string& b) { cleanEffect<Echo>(a, b); }
		));

		module.set("Random", PlayingMode::Random);
		module.set("Orderly", PlayingMode::Orderly);
		module.set("Single", PlayingMode::Single);

		module.set("Stopped", Status::Stopped);
		module.set("Paused", Status::Paused);
		module.set("Playing", Status::Playing);

		script.initClasses<MusicBase>();
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
		for (auto& it : _currentPlaylistKeys) {
			if (it != 0)
				_playlists[it]->update(deltaTime);
		}
	}

	MusicBase* Music::loadMusic(const std::string& name) 
	{
		auto ratMusic = _assets.load(name);

		if (ratMusic != nullptr) {
			auto base = new MusicBase(ratMusic);
			_musicHolder.emplace_back(base);

			LOG_INFO("[Music] Created instance of ", name);

			return base;
		}

		return nullptr;
	}

	void Music::removeMusic(MusicBase* base)
	{
		_musicHolder.remove_if([=](auto it){ 
            bool same = (it == base);
            if (same) {
				_assets.unload(base->getSource());
                LOG_INFO("[Music] Removed instance of ", base->getName());
                delete base;
            }
            return same;
        });
	}

	void Music::addPlaylist(const std::string& key, const std::vector<std::string>& newPlaylist)
	{
		_playlists[fnv1a_32(key.c_str())] = std::make_unique<Playlist>(getModule<AudioEffects>());

		for (auto it : newPlaylist)
			addToPlaylist(key, it);
	}

	void Music::addToPlaylist(const std::string& key, const std::string& name)
	{
		auto ratMusic = _assets.load(name);

		if (ratMusic != nullptr) {
			auto&& base = MusicBase(ratMusic);
			_playlists[fnv1a_32(key.c_str())]->add(std::move(base));

			LOG_INFO("Added ", name, " to playlist ", key);
		}
	}

	void Music::removeFromPlaylist(const std::string& key, const std::string& name)
	{
		auto hashKey = fnv1a_32(key.c_str());

		for (auto& it : _currentPlaylistKeys) {
			if (it == hashKey)
				it = 0;
		}

		auto log = [&](const std::string& name) {
			LOG_INFO("Removed ", name, " form playlist ", key);
		};

		if (name.empty()) {
			for (auto& it : _playlists[hashKey]->getContainerRef()) {
				auto name = it->getName();
				_assets.unload(it->getSource());
				log(name);
			}

			_playlists.erase(hashKey);
		}
		else {
			auto& playlist = _playlists[hashKey]->getContainerRef();
			auto& music = playlist[_playlists[hashKey]->getID(name)];
			auto source = music->getSource();
			_playlists[hashKey]->remove(name);
			_assets.unload(source);
			log(name);
		}
	}

	void Music::play(unsigned int musicTrack, const std::string& key, const std::string& name)
	{
		auto hashKey = fnv1a_32(key.c_str());
		auto samePlaylist = (_currentPlaylistKeys[musicTrack] == hashKey);

		for (unsigned int i = 0; i < 3; ++i) {
			if (_currentPlaylistKeys[i] == hashKey && musicTrack != i) {
				LOG_INFO("Can't play the same playlist on several tracks at the same time!");
				return;
			}
		}

		if (_currentPlaylistKeys[musicTrack] == 0 || (name == "" && samePlaylist))
			_playlists[hashKey]->play(name);
		else {
			auto currentPlaying = _playlists[_currentPlaylistKeys[musicTrack]]->getCurrentPlaying();

			if (currentPlaying->getName() != name || (_playlists[_currentPlaylistKeys[musicTrack]]->getStatus() == Status::Stopped && samePlaylist))
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
			for (auto& it : _currentPlaylistKeys) {
				if (it != 0)
					_playlists[it]->pause();
			}
		}
		else if (musicTrack < 3 && musicTrack >= 0 && _currentPlaylistKeys[musicTrack] != 0) {
			_playlists[_currentPlaylistKeys[musicTrack]]->pause();
		}
	}

	void Music::stop(int musicTrack)
	{
		if (musicTrack == -1) {
			for (auto& it : _currentPlaylistKeys) {
				if (it != 0)
					_playlists[it]->stop();
			}
		}
		else if (musicTrack < 3 && musicTrack >= 0 && _currentPlaylistKeys[musicTrack] != 0) {
			_playlists[_currentPlaylistKeys[musicTrack]]->stop();
		}
	}

	Music::Status Music::getStatus(unsigned int musicTrack, const std::string& name)
	{
		return _playlists[_currentPlaylistKeys[musicTrack]]->getStatus(name);
	}

	Music::Status Music::getStatus(const std::string& key, const std::string& name)
	{
		return _playlists[fnv1a_32(key.c_str())]->getStatus(name);
	}

	MusicBase* Music::getCurrentPlaying(unsigned int musicTrack)
	{
		if (musicTrack < 3 && _currentPlaylistKeys[musicTrack] != 0) {
			return _playlists[_currentPlaylistKeys[musicTrack]]->getCurrentPlaying().get();
		}
		return nullptr;
	}

	bool Music::includes(const std::string& key, const std::string& name)
	{
		return _playlists[fnv1a_32(key.c_str())]->includes(name);
	}

	void Music::setPlayingMode(const std::string& key, PlayingMode mode)
	{
		switch (mode) {
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


	float Music::getVolume(const std::string& key, const std::string& name)
	{
		if (name.empty() && key.empty())
			return RatMusic::GetMusicVolume();

		return _playlists[fnv1a_32(key.c_str())]->getVolume(name);
	}

	float Music::getVolume(int musicTrack, const std::string& name)
	{
		if (name.empty() && musicTrack == -1)
			return RatMusic::GetMusicVolume();

		return _playlists[_currentPlaylistKeys[musicTrack]]->getVolume(name);
	}

	void Music::setGlobalEffects()
	{
		for (auto& it : _currentPlaylistKeys) {
			if (it != 0)
				_playlists[it]->setGlobalEffects();
		}
	}

	void Music::cleanEffects()
	{
		for (auto i = 0; i <= 3; ++i) {
			if (_currentPlaylistKeys[i] != 0)
				getCurrentPlaying(i)->getSource()->cleanAllEffects();
		}
	}

}
