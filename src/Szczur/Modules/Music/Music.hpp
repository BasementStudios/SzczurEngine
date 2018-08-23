#pragma once

#include <memory> // unique_ptr
#include <array>
#include <string>
#include <vector>

#include <boost/container/flat_map.hpp>

#include "Szczur/Utility/Modules/Module.hpp"
#include "Szczur/Utility/Convert/Hash.hpp"
#include "Szczur/Modules/Script/Script.hpp"
 
#include "MusicAssets.hpp"
#include "Playlist.hpp"

namespace rat 
{
	class Music : public Module<Script, AudioEffects>
	{ 
		using PlaylistContainer_t = boost::container::flat_map<Hash32_t, std::unique_ptr<Playlist>>;

	public:

		using PlayingMode = Playlist::PlayingMode;
		using Status = Playlist::Status;

	private:

		PlaylistContainer_t _playlists;
		MusicAssets _assets;
		
		std::array<Hash32_t, 3> _currentPlaylistKeys {0, 0, 0};

	public:

		Music();
		~Music();

		void initScript();

		void update(float deltaTime);

		void loadPlaylistFromJson(const std::string& fileName);

		void addPlaylist(const std::string& key, const std::vector<std::string>& newPlaylist);
		const Playlist::Container_t& getPlaylist(const std::string& key);

		void addToPlaylist(const std::string& key, const std::string& name);
		void removeFromPlaylist(const std::string& key, const std::string& name = "");

		void play(unsigned int musicTrack, const std::string& key, const std::string& name = "");
		void pause(int musicTrack = -1);
		void stop(int musicTrack = -1);

		Status getStatus(unsigned int musicTrack, const std::string& name = "");
		Status getStatus(const std::string& key, const std::string& name = "");

		RatMusic* getCurrentPlaying(unsigned int musicTrack);

		bool includes(const std::string& key, const std::string& name);

		void setPlayingMode(const std::string& key, PlayingMode mode);

		void setVolume(float volume, const std::string& key = "", const std::string& name = "");
		float getVolume(const std::string& key = "", const std::string& name = "");
		float getVolume(int musicTrack = -1, const std::string& name = "");

		template <typename T>
		T& getEffect(unsigned int musicTrack, const std::string& name);
		template <typename T>
		T& getEffect(const std::string& key, const std::string& name);

		template <typename T>
    	void cleanEffect(unsigned int musicTrack, const std::string& name);
		template <typename T>
    	void cleanEffect(const std::string& key, const std::string& name);
		void cleanEffects();

		void setGlobalEffects();

	private:

		void unLoad(const std::string& name);

	};

	template <typename T>
	T& Music::getEffect(const std::string& key, const std::string& name)
	{
		auto& playlist = _playlists[fnv1a_32(key.c_str())];
		auto containerRef = playlist->getContainerRef();
		return containerRef[playlist->getID(name)]->getSource()->getEffect<T>();
	}

	template <typename T>
	T& Music::getEffect(unsigned int musicTrack, const std::string& name)
	{
		auto& playlist = _playlists[_currentPlaylistKeys[musicTrack]];
		auto containerRef = playlist->getContainerRef();
		return containerRef[playlist->getID(name)]->getSource()->getEffect<T>();
	}

    template <typename T>
    void Music::cleanEffect(const std::string& key, const std::string& name)
    {
		auto& playlist = _playlists[fnv1a_32(key.c_str())];
		auto containerRef = playlist->getContainerRef();
        containerRef[playlist->getID(name)]->getSource()->cleanEffect<T>();
    }

	template <typename T>
    void Music::cleanEffect(unsigned int musicTrack, const std::string& name)
    {
		auto& playlist = _playlists[_currentPlaylistKeys[musicTrack]];
		auto containerRef = playlist->getContainerRef();
        containerRef[playlist->getID(name)]->getSource()->cleanEffect<T>();
    }

}