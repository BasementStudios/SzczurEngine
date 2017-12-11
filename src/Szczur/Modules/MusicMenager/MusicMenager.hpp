#pragma once

#include <memory>

#include "Szczur/Utility/Modules.hpp"

#include "Playlist.hpp"

namespace rat {
    class MusicMenager : public ModuleBase<> { 
		using ModuleBase::ModuleBase;
		using Container_t = std::vector<std::shared_ptr<Playlist>>;

	private:

		Container_t _playlists;
		
	public:

		void init();
		void input(const sf::Event& event);
		void render();
		void update(float deltaTime);

		void push(std::vector<std::string> newPlaylist, int pos = -1);

		void remove(int id);

		std::shared_ptr<Playlist> operator[](int id);
		
	};
}



















