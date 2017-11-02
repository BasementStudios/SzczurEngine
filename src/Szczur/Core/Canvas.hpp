#pragma once

#include <boost/container/flat_map.hpp>

#include <SFML/Graphics.hpp>

#include "Szczur/Utils/Hash.hpp"
#include "Szczur/Utils/ModuleBase.hpp"
#include "Graphics/Layer.hpp"

namespace rat {
	class Canvas : public ModuleBase<> { using ModuleBase::ModuleBase;
	public:

		using Holder_t = boost::container::flat_map<rat::Hash32_t, std::unique_ptr<rat::Layer>>;

	private:

		Holder_t _layers;
		sf::Vector2u _size;

	public:

		void init();

		void addLayer(rat::Hash32_t layerId);

		void resize(const sf::Vector2u& size);
		void resize(const sf::RenderWindow& window);

		void display(sf::RenderTarget& target, rat::Hash32_t layerId);

		void render(sf::Drawable& drawable, rat::Hash32_t layerId);
		void render(Drawable& drawable, rat::Hash32_t layerId);

		Layer& getLayer(rat::Hash32_t layerId);
		const Layer& getLayer(rat::Hash32_t layerId) const;

		sf::Vector2u getLayersSize() const;
	};
}
