// Main class for drawing part of texture on screen

#pragma once

#include <SFML/Graphics.hpp>

#include "../Core/DataTypes/Texture.h"
#include "../Core/DataTypes/Drawable.h"

namespace rat {
	class Sprite : public Drawable {
	private:
		const Texture* _textureHandler;
		int _frame;
		sf::Sprite _sprite;
	
	public:
	// Init
		Sprite() :
			_textureHandler(nullptr), _frame(-1) {
		}
		Sprite(const Texture& texture, int frame=0) :
			_frame(-1) {			
			setTexture(texture);			
			setFrame(frame);
		}
		
	// Getters
		sf::Sprite& base() {
			return _sprite;
		}
		// Returns max number of frames for handled texture
		int getFramesNumber() {
			return _textureHandler->getFrames().x * _textureHandler->getFrames().y;
		}
	
	// Setters
		void setFrame(int frame) {
			if(frame != _frame) {
				_frame = frame;
				int startX = _frame % _textureHandler->getFrames().x;
				int startY = _frame / _textureHandler->getFrames().x;
				const sf::Vector2i& frameSize = _textureHandler->getFrameSize();
				_sprite.setTextureRect(sf::IntRect(frameSize.x*startX, frameSize.y*startY, frameSize.x, frameSize.y));
				// _sprite.setTextureRect(sf::IntRect(512, 0, 128, 128));
				// std::cout<<frameSize.x*startX<<' '<<frameSize.y*startY<<' '<<frameSize.x<<' '<<frameSize.y<<'\n';
			}
		}
		void setTexture(const Texture& texture) {
			_textureHandler = &texture;
			_sprite.setTexture(_textureHandler->getTexture());
		}
		void set(const Texture& texture, int frame=0) {
			setTexture(texture);		
			setFrame(frame);
		}
		
	// Utilites
		void draw(Layer& layer) const override {
			layer.draw(_sprite);			
		}
	};
}