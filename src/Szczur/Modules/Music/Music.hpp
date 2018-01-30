#pragma once

#include <memory>

#include "Szczur/Utility/Modules.hpp"

#include "Playlist.hpp"

namespace rat 
{
	class Music : public ModuleBase<>
	{ 
		using ModuleBase::ModuleBase;
		using Container_t = std::vector<std::shared_ptr<Playlist>>;

	private:

		Container_t _playlists;
		
		int _currentPlaylistID = -1;

	public:

		void init();
		void input(const sf::Event& event);
		void render();
		void update(float deltaTime);

		void push(const std::vector<std::string>& newPlaylist, int pos = -1);

		void remove(unsigned int id);

		void play(unsigned int id, const std::string& fileName = "");

		std::shared_ptr<Playlist> operator[](unsigned int id);

	};
}



















