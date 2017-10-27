#pragma once

#include <SFML/Graphics.hpp>

#include "Texture.hpp"
#include "Layer.hpp"

namespace rat {
	class Sprite : public Drawable {
	private:
		const Texture* _textureHandler = nullptr;
		int _frame = -1;
		sf::Sprite _sprite;

	public:
	// Init
		Sprite() = default;
		Sprite(const Texture& texture, int frame=0) {
			set(texture, frame);
		}

	// Getters
		sf::Sprite& base() {
			return _sprite;
		}
		const sf::Sprite& base() const {
			return _sprite;
		}
		// Returns max number of frames for handled texture
		int getFramesNumber() {
			return _textureHandler->getFramesNumber();
		}

	// Setters
		void set(const Texture& texture, int frame=0) {
			_textureHandler = &texture;
			_sprite.setTexture(_textureHandler->getTexture());
			setFrame(frame);
		}
		void setFrame(int frame) {
			if(frame != _frame) {
				_frame = frame % _textureHandler->getFramesNumber();
				int startX = _frame % _textureHandler->getFrames().x;
				int startY = _frame / _textureHandler->getFrames().x;
				const sf::Vector2i& frameSize = _textureHandler->getFrameSize();
				_sprite.setTextureRect(sf::IntRect(frameSize.x*startX, frameSize.y*startY, frameSize.x, frameSize.y));
				// _sprite.setTextureRect(sf::IntRect(512, 0, 128, 128));
				// std::cout<<frameSize.x*startX<<' '<<frameSize.y*startY<<' '<<frameSize.x<<' '<<frameSize.y<<'\n';
			}
		}
		void setOrigin(float ox, float oy) {
			_sprite.setOrigin(ox, oy);
		}
		void setOrigin(const sf::Vector2f& origin) {
			_sprite.setOrigin(origin);
		}
		void centerOrigin() {
			const sf::Vector2i& frameSize = _textureHandler->getFrameSize();
			_sprite.setOrigin(frameSize.x/2.f, frameSize.y/2.f);
		}

	// Utilites
		void draw(Layer& layer) const override {
			layer.draw(_sprite);
		}
	};
}
