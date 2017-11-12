#pragma once

class Mesh
{
public:
	const sf::Texture*									texture;

	std::vector<std::shared_ptr<sf::Vertex>>			vertices;
	std::vector<sf::Vertex*>							verticesDisplay;

public:
	Mesh()
	{
		texture = nullptr;
	}

	~Mesh() = default;

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

