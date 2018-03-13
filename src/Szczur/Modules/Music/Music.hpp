#pragma once

#include <memory>

#include "Szczur/Utility/Modules/Module.hpp"
#include "Szczur/Debug.hpp"

#include "MusicAssets.hpp"
#include "Playlist.hpp"

namespace rat 
{
	class Music : public Module<>
	{ 
		using PlaylistContainer_t = std::vector<std::unique_ptr<Playlist>>;

	public:

		using PlayingMode = Playlist::PlayingMode;

	private:

		PlaylistContainer_t _playlists;
		MusicAssets _assets;
		
		int _currentPlaylistID = -1;

	public:

		void update(float deltaTime);

		void addPlaylist(const std::vector<std::string>& newPlaylist);

		void addToPlaylist(unsigned int id, const std::string& fileName);
		void removeFromPlaylist(unsigned int id, const std::string& fileName = "");

		void play(unsigned int id, const std::string& fileName = "");
		void pause();
		void stop();

		bool includes(unsigned int id, const std::string& fileName) const;

		void setPlayingMode(unsigned int id, PlayingMode mode);

		void setVolume(unsigned int id, float volume, const std::string& fileName = "");
		float getVolume(const std::string& fileName);

		template <typename T>
		T& getEffect(const std::string& fileName);

		template <typename T>
    	void cleanEffect(const std::string& fileName);

	private:

		void unLoad(const std::string& fileName);

	};

	template <typename T>
	T& Music::getEffect(const std::string& fileName)
	{
		return _assets.get(fileName).getEffect<T>();
	}

    template <typename T>
    void Music::cleanEffect(const std::string& fileName)
    {
        _assets.get(fileName).cleanEffect<T>();
    }

}



















