#pragma once

#include <array>
#include <memory>

#include <SFML/Graphics.hpp>

#include "RenderLayer.hpp"
#include "Szczur/Utility/Module.hpp"
#include "Szczur/Modules/Window/Window.hpp"

namespace rat
{

class Canvas : public Module<Window>
{
public:

	struct LayerID
	{
		enum Code : size_t
		{
			Back, Game,
			Count
		};
	};

	using Holder_t = std::array<std::unique_ptr<RenderLayer>, LayerID::Count>;

	template <typename Tuple>
	Canvas(Tuple&& tuple);

	Canvas(const Canvas&) = delete;

	Canvas& operator = (const Canvas&) = delete;

	Canvas(Canvas&&) = delete;

	Canvas& operator = (Canvas&&) = delete;

	~Canvas();

	void clear();

	void render();

	void recreateLayers();

	sf::RenderWindow& getWindow();
	const sf::RenderWindow& getWindow() const;

	RenderLayer& getLayer(LayerID::Code id);
	const RenderLayer& getLayer(LayerID::Code id) const;

	void draw(LayerID::Code id, const sf::Drawable& drawable, const sf::RenderStates& states = sf::RenderStates::Default);
	void draw(LayerID::Code id, const sf::Vertex* vertices, size_t vertexCount, sf::PrimitiveType type, const sf::RenderStates& states = sf::RenderStates::Default);

private:

	Holder_t _layers;

};

template <typename Tuple>
Canvas::Canvas(Tuple&& tuple) :
	Module(tuple)
{
	for (auto& av : _layers)
		av.reset(new RenderLayer(getModule<Window>().getWindow().getSize()));

	LOG_CONSTRUCTOR();
}

}
