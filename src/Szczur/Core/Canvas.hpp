#pragma once

#include <boost/container/flat_map.hpp>

#include <SFML/Graphics.hpp>

#include "Szczur/Utils/Hash.hpp"
#include "Szczur/Utils/Modules.hpp"
#include "Graphics/RenderLayer.hpp"

namespace rat {
	class Canvas : public ModuleBase<> { using ModuleBase::ModuleBase;
	public:

		using Holder_t = boost::container::flat_map<rat::Hash32_t, std::unique_ptr<rat::RenderLayer>>;

	private:

		sf::RenderWindow* _windowPtr;
		Holder_t _layers;

	public:

		Canvas() = default;

		void init(sf::RenderWindow* windowPtr);

		void recreateLayers();

		void addLayer(rat::Hash32_t layerId, std::size_t priority);

		void removeLayer(rat::Hash32_t layerId);

		rat::RenderLayer& getLayer(rat::Hash32_t layerId);
		const rat::RenderLayer& getLayer(rat::Hash32_t layerId) const;

		void draw(rat::Hash32_t layerId, const sf::Drawable& drawable, const sf::RenderStates& states = sf::RenderStates::Default);

		void display();
	};
}
