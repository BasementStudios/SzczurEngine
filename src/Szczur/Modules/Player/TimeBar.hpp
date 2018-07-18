#pragma once
#include "SFML/Graphics.hpp"
#include <vector>

namespace rat {
	class ImageWidget; class Widget; class GUI;
	class TimeBar 
	{
	public:
		TimeBar();

		void initGUI(GUI& gui);
		void setParent(Widget* newParent);

		void setTimeBarIndex(int index);
		void setMaxTime(int newMaxHP);
		void setCurrentTime(int newCurrentHP);

		void setPropSize(sf::Vector2f size);
		void setPropPosition(sf::Vector2f pos);
	private:
		void _recalcBar();

		float _maxTime;
		float _currentTime;
		int _currentBar;

		sf::Vector2f _size;

		std::vector<sf::Texture*> _foregroundTexture;
		std::vector<sf::Texture*> _backgroundTexture;
		std::vector<sf::Texture*> _fullBarTexture;

		Widget* _base = nullptr;
		ImageWidget* _backgroundTime = nullptr;
		ImageWidget* _foregroundTime = nullptr;
	};
}