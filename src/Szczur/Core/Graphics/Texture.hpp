#pragma once

#include <SFML/Graphics.hpp>

namespace rat {
	class Texture {
	private:
		sf::Texture _texture;
		sf::Vector2i _frames; // Max number of horizontal/vertical frames (for sprites)

		sf::Vector2i _frameSize;
		int _framesNumber;

	public:
	// Init
		Texture() :
			_frames(1, 1) {
		}
		Texture(const std::string& filename) {
			loadFromFile(filename, 1, 1);
		}
		Texture(const std::string& filename, int horiz, int vert) {
			loadFromFile(filename, horiz, vert);
		}
		Texture(const std::string& filename, const sf::Vector2i& frames) {
			loadFromFile(filename, frames);
		}

	// Setters
		void setFrames(int horiz, int vert) {
			_frames = sf::Vector2i(horiz, vert);
			_frameSize = sf::Vector2i(
				_texture.getSize().x / horiz,
				_texture.getSize().y / vert
			);
			_framesNumber = _frames.x * _frames.y;
		}
		void setFrames(const sf::Vector2i& frames) {
			_frames = frames;
			_frameSize = sf::Vector2i(
				_texture.getSize().x / _frames.x,
				_texture.getSize().y / _frames.y
			);
			_framesNumber = _frames.x * _frames.y;
		}

	// Loading
		bool loadFromFile(const std::string& filename, int horiz, int vert) {
			bool ret = _texture.loadFromFile(filename);
			setFrames(horiz, vert);
			return ret;
		}
		bool loadFromFile(const std::string& filename, const sf::Vector2i& frames) {
			bool ret = _texture.loadFromFile(filename);
			setFrames(frames);
			return ret;
		}

	// Getters
		sf::Vector2i getFrames() const {
			return _frames;
		}
		const sf::Texture& getTexture() const {
			return _texture;
		}
		sf::Vector2i getFrameSize() const {
			return _frameSize;
		}
		int getFramesNumber() const {
			return _framesNumber;
		}
	};
}
