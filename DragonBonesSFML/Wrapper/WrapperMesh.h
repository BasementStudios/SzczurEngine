#pragma once

class WrapperMesh
{
public:
	const sf::Texture*				texture;

	sf::Vertex*						vertices;
	int								vertCount;


public:
	WrapperMesh()
	{
		vertices = nullptr;
		texture = nullptr;
		vertCount = 0;
	}

	~WrapperMesh() = default;

	void render(sf::RenderTarget& target, sf::RenderStates states)
	{
		if (texture != nullptr && vertices != nullptr && vertCount > 0)
		{
			states.texture = texture;

			target.draw(vertices, vertCount, sf::PrimitiveType::TriangleFan, states);
		}
	}
};

