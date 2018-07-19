#pragma once
#include "SFML/Graphics.hpp"
#include <vector>

namespace rat {
	class ImageWidget; class Widget; class GUI;
	class HPBar
	{
	public:
		HPBar();

		void initGUI(GUI& gui);
		void setParent(Widget* newParent);

		void setHPBarIndex(int index);
		void setMaxHP(int newMaxHP);
		void setCurrentHP(int newCurrentHP);

		void setPropSize(sf::Vector2f size);
		void setPropPosition(sf::Vector2f pos);
		void setStatus(sf::Texture*);
		void removeStatus();
	private:
		void _recalcBar();

		float _maxHP;
		float _currentHP;
		int _currentBar;

		sf::Vector2f _size;

		std::vector<sf::Texture*> _foregroundTexture;
		std::vector<sf::Texture*> _backgroundTexture;

		Widget* _base = nullptr;
		ImageWidget* _backgroundHeart = nullptr;
		ImageWidget* _status = nullptr;
		ImageWidget* _foregroundHeart = nullptr;
	};
}