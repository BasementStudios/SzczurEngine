#pragma once
#include "Szczur/Modules/GUI/Base/BaseBar.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

namespace rat {
	class ImageWidget; class WearableItem; class Equipment;
	class Necklace : public BaseBar
	{
	public:
		Necklace(sf::Texture* necklaceText, Equipment* eq);

		bool addStone(WearableItem* stone);
		bool removeStone(WearableItem* stone);

		bool hasStone(const std::string& name);

		void reset();

		void setPropSize(float, float);
		//void setPropPosition(float, float);
	private:
		struct stone {
			ImageWidget* image;
			WearableItem* item;
		};

		ImageWidget* _necklaceImage = nullptr;
		std::vector<stone> _stonesImages;

		Equipment* _equipment;
	};
}
