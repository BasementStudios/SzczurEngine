#pragma once

#include <SFML/Graphics.hpp>

#include <m2/Utility/Hash.hpp>
#include <m2/Utility/Modules.hpp>
#include <m2/Container/FlatMap.hpp>

#include "Graphics/RenderLayer.hpp"

namespace rat {
	class Canvas : public m2::utility::ModuleBase<> { using ModuleBase::ModuleBase;
	public:

		using Holder_t = m2::container::FlatMap<m2::utility::Hash32_t, std::unique_ptr<rat::RenderLayer>>;

	private:

		sf::RenderWindow* _windowPtr;
		Holder_t _layers;

	public:

		Canvas() = default;

		void init(sf::RenderWindow* windowPtr);

		void recreateLayers();

		void addLayer(m2::utility::Hash32_t layerId, std::size_t priority);

		void removeLayer(m2::utility::Hash32_t layerId);

		rat::RenderLayer& getLayer(m2::utility::Hash32_t layerId);
		const rat::RenderLayer& getLayer(m2::utility::Hash32_t layerId) const;

		void draw(m2::utility::Hash32_t layerId, const sf::Drawable& drawable, const sf::RenderStates& states = sf::RenderStates::Default);

		void display();
	};
}
