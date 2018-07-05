#pragma once 
#include "Drawable.hpp" 
#include "VertexArray.hpp" 
#include "Texture.hpp" 

namespace sf3d {
	class SimpleSprite : public Drawable {
	public:
		void setTexture(Texture* texture);

		virtual void draw(RenderTarget& target, RenderStates states) const override;
	private:
		Texture * _texture{nullptr};
		VertexArray _vertices{4u};
	};
}