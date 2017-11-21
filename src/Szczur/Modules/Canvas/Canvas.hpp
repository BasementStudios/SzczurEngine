#pragma once

#include <array>
#include <memory>

#include <SFML/Graphics.hpp>

#include "RenderLayer.hpp"
#include "Szczur/Utility/Modules.hpp"

namespace rat {
	class Canvas : public ModuleBase<> { using ModuleBase::ModuleBase;
	public:

		// TODO: Add all ids
		enum class LayerId : size_t {
			Back, Game, GUI,
			Count
		};

		using Holder_t = std::array<std::unique_ptr<rat::RenderLayer>, (size_t)LayerId::Count>;

	private:

		sf::RenderWindow* _windowPtr;
		Holder_t _layers;

	public:

		void init(sf::RenderWindow* windowPtr);

		void recreateLayers();

		sf::RenderWindow& getWindow();
		const sf::RenderWindow& getWindow() const;

		rat::RenderLayer& getLayer(LayerId id);
		const rat::RenderLayer& getLayer(LayerId id) const;

		void draw(LayerId id, const sf::Drawable& drawable, const sf::RenderStates& states = sf::RenderStates::Default);
		void draw(LayerId id, const sf::Vertex* vertices, size_t vertexCount, sf::PrimitiveType type, const sf::RenderStates& states = sf::RenderStates::Default);

		void display();
	};
}