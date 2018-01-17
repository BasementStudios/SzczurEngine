#pragma once

#include <array>
#include <memory>

#include <SFML/Graphics.hpp>

#include "RenderLayer.hpp"
#include "Szczur/Utility/Modules.hpp"
#include "Szczur/Modules/Window/Window.hpp"

namespace rat
{

class Canvas : public Module<Window>
{
public:

	enum class LayerId : size_t
	{
		Back, Game,
		Count
	};

	using Holder_t = std::array<std::unique_ptr<rat::RenderLayer>, (size_t)LayerId::Count>;

	template <typename Tuple>
	Canvas(Tuple&& tuple) :
		Module(tuple), _window(getModule<Window>().getWindow())
	{
		for (auto& av : _layers)
			av.reset(new RenderLayer(_window.getSize()));

		LOG_INFO(this, " -> Module Canvas created");
	}

	Canvas(const Canvas&) = delete;

	Canvas& operator = (const Canvas&) = delete;

	Canvas(Canvas&&) = delete;

	Canvas& operator = (Canvas&&) = delete;

	~Canvas()
	{
		LOG_INFO(this, " -> Module Canvas destructed");
	}

	void update(float deltaTime);

	void render();

	void recreateLayers();

	sf::RenderWindow& getWindow();
	const sf::RenderWindow& getWindow() const;

	rat::RenderLayer& getLayer(LayerId id);
	const rat::RenderLayer& getLayer(LayerId id) const;

	void draw(LayerId id, const sf::Drawable& drawable, const sf::RenderStates& states = sf::RenderStates::Default);
	void draw(LayerId id, const sf::Vertex* vertices, size_t vertexCount, sf::PrimitiveType type, const sf::RenderStates& states = sf::RenderStates::Default);

private:

	sf::RenderWindow& _window;
	Holder_t _layers;

};

}
