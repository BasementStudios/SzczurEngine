#pragma once

#include <fstream>

#include <SFML/Graphics.hpp>

#include <boost/container/flat_map.hpp>

#include "Szczur/Utility/Hash.hpp"
#include "Szczur/Utility/Modules.hpp"

#include "Graphics/Texture.hpp"

namespace rat {
	class AssetsLoader : public ModuleBase<> { using ModuleBase::ModuleBase;
	public:

		using Holder_t = boost::container::flat_map<Hash32_t, std::unique_ptr<rat::Texture>>;

	private:

		Holder_t _textures;

	public:

		void init();

		Texture& getTexture(Hash32_t textureId);
		const Texture& getTexture(Hash32_t textureId) const;

		void loadNewTexture(const std::string& filename, int horiz, int vert);
		void loadNewTexture(const std::string& filename, const sf::Vector2i& frames);

		bool loadTexturesFromDataFile(const std::string& dataPath);

		void loadTexturesFromDataDirectories(const std::string& dataDirectoriesPath);
	};
}
