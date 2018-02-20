#pragma once

#include <memory>

#include "Szczur/Utility/Module.hpp"
#include "Szczur/Modules/Assets/Assets.hpp"

#include "Playlist.hpp"

namespace rat 
{
	class Music : public Module<Assets>
	{ 
		using MusicPointer_t = std::unique_ptr<sf::Music>;
		using Container_t = std::vector<std::unique_ptr<Playlist>>;

	public:

		using PlayingMode = Playlist::PlayingMode;

	private:

		Container_t _playlists;
		
		int _currentPlaylistID = -1;

	public:

		template <typename Tuple>
    	Music(Tuple&& tuple);

		void update(float deltaTime);

		void addPlaylist(const std::vector<std::string>& newPlaylist);

		void add(unsigned int id, const std::string& fileName);
		void remove(unsigned int id, const std::string& fileName = "");

		void play(unsigned int id, const std::string& fileName = "");
		void pause(unsigned int id);
		void stop(unsigned int id);

		bool includes(unsigned int id, const std::string& fileName) const;

		void setPlayingMode(unsigned int id, PlayingMode mode);

		void setVolume(unsigned int id, float volume, const std::string& fileName = "");
		float getVolume(unsigned int id, const std::string& fileName) const;

	private:

		std::string getPath(const std::string& fileName) const;

		bool usingByOtherPlaylist(unsigned int id, const std::string& fileName) const;

		void unLoad(unsigned int id, const std::string& fileName);

	};

	template <typename Tuple>
    Music::Music(Tuple&& tuple)
		: Module(tuple)
	{
		LOG_CONSTRUCTOR();
	}
}



















