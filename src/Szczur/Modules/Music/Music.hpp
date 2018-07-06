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

	private:

		PlaylistContainer_t _playlists;
		MusicAssets _assets;
		
		std::array<Hash32_t, 3> _currentPlaylistKeys {0, 0, 0};

	public:

		Music();
		~Music();

		void initScript();

		MusicAssets* getAssetsManager();

		void update(float deltaTime);

		void loadPlaylistFromJson(const std::string& fileName);

		void addPlaylist(const std::string& key, const std::vector<std::string>& newPlaylist);
		const Playlist::Container_t& getPlaylist(const std::string& key);

		void addToPlaylist(const std::string& key, const std::string& name);
		void removeFromPlaylist(const std::string& key, const std::string& name = "");

		void play(unsigned int musicTrack, const std::string& key, const std::string& name = "");
		void pause(int musicTrack = -1);
		void stop(int musicTrack = -1);

		RatMusic* getCurrentPlaying(unsigned int musicTrack);
		RatMusic& get(const std::string& name);

		bool includes(const std::string& key, const std::string& name);

		void setPlayingMode(const std::string& key, PlayingMode mode);

		void setVolume(float volume, const std::string& key = "", const std::string& name = "");
		float getVolume(const std::string& name);

		template <typename T>
		T& getEffect(const std::string& name);

		template <typename T>
    	void cleanEffect(const std::string& name);
		void cleanEffects();

		void setGlobalEffects();

	private:

		void unLoad(const std::string& name);

	};

	template <typename T>
	T& Music::getEffect(const std::string& name)
	{
		return _assets.get(name).getEffect<T>();
	}

    template <typename T>
    void Music::cleanEffect(const std::string& name)
    {
        _assets.get(name).cleanEffect<T>();
    }

}