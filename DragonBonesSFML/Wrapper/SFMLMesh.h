#pragma once

#include <SFML\Graphics.hpp>

DRAGONBONES_NAMESPACE_BEGIN

class SFMLMesh
{
public:
	const sf::Texture*									texture;

	std::vector<std::shared_ptr<sf::Vertex>>			vertices;
	std::vector<sf::Vertex*>							verticesDisplay;

public:
	SFMLMesh()
	{
		texture = nullptr;
	}

	~SFMLMesh() = default;

	void render(sf::RenderTarget& target, sf::RenderStates states)
	{
		if (texture != nullptr && verticesDisplay.size() > 0)
		{
			states.texture = texture;

			std::vector<sf::Vertex> verts;
			verts.reserve(verticesDisplay.size());
			std::transform(std::begin(verticesDisplay), std::end(verticesDisplay), std::back_inserter(verts), [] (sf::Vertex* item) { return *item; });

			target.draw(&verts[0], verts.size(), sf::PrimitiveType::Triangles, states);
		}
	}
};

DRAGONBONES_NAMESPACE_END
