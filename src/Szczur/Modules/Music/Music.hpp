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
    	Music(Tuple&& tuple)
			: Module(tuple)
		{
			
		}

		void update(float deltaTime);

		void addPlaylist(const std::vector<std::string>& newPlaylist);

		void remove(unsigned int id);

		void play(unsigned int id, const std::string& fileName = "");

		Playlist& operator[](unsigned int id);

	private:

		std::string getPath(const std::string& fileName) const;

	};
}



















