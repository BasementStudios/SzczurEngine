#pragma once

#include <fstream>

#include <SFML/Graphics.hpp>

#include <m2/Utility/Hash.hpp>
#include <m2/Utility/Modules.hpp>
#include <m2/Container/FlatMap.hpp>

#include "Graphics/Texture.hpp"

namespace rat {
	class AssetsLoader : public m2::utility::ModuleBase<> { using ModuleBase::ModuleBase;
	public:

		using Holder_t = m2::container::FlatMap<m2::utility::Hash32_t, std::unique_ptr<rat::Texture>>;

	private:

		Holder_t _textures;

	public:

		void init();

		Texture& getTexture(m2::utility::Hash32_t textureId);
		const Texture& getTexture(m2::utility::Hash32_t textureId) const;

		void loadNewTexture(const std::string& filename, int horiz, int vert);
		void loadNewTexture(const std::string& filename, const sf::Vector2i& frames);

		bool loadTexturesFromDataFile(const std::string& dataPath);

		void loadTexturesFromDataDirectories(const std::string& dataDirectoriesPath);
	};
}
