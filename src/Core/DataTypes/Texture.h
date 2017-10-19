// This class keep texture loaded from files
// It's simple wrapper for sf::Texture with some utilities

#include <iostream>
#include <fstream>

#include <SFML/Graphics.hpp>

namespace rat {
	class Texture {
	private:
		sf::Texture _texture;
		sf::Vector2i _frames; // Max number of horizontal/vertical frames (for sprites)
		
	public:
	// Init
		Texture() :
			_frames(0,0) {
		}
		Texture(const std::string& filename) {
			loadFromFile(filename);
		}
		Texture(const std::string& filename, int horiz, int vert) {
			loadFromFile(filename, horiz, vert);
		}
		Texture(const std::string& filename, const sf::Vector2i& frames) {
			loadFromFile(filename, frames);
		}
		
	// Setters
		bool loadFromFile(const std::string& filename) {
			return _texture.loadFromFile(filename);
		}
		void setFrames(int horiz, int vert) {
			_frames.x = horiz;
			_frames.y = vert;
		}
		void setFrames(const sf::Vector2i& frames) {
			_frames = frames;
		}
		
	// Modifiers
		bool loadFromFile(const std::string& filename, int horiz, int vert) {
			setFrames(horiz, vert);
			return loadFromFile(filename);
		}		
		bool loadFromFile(const std::string& filename, const sf::Vector2i& frames) {
			setFrames(frames);
			return loadFromFile(filename);
		}
		
		// Getters
		sf::Vector2i getFrames() const {
			return _frames;
		}
		const sf::Texture& getTexture() const {
			return _texture;
		}
	};
}