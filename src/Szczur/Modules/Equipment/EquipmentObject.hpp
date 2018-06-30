#pragma once
#include <SFML/Graphics.hpp>
#include "Szczur/Modules/GUI/Base/BaseBar.hpp"
#include <Sol/sol.hpp>
#include <memory>
/*
	class for items containing image widget of object, name and description
*/

namespace rat {


	class ImageWidget;

	class EquipmentObject {
	public:
		EquipmentObject(std::string nameId);
		sf::Texture* getTexture();
		std::string getName();
		std::string getDescription();
		std::string getNameId();
		
		//cos z tekstura
		void setName(std::string name);
		void setDescription(std::string description);
		//void setIcon(sf::Texture* text);
		void setIcon(std::string path);

		virtual ~EquipmentObject() {}

		/*bool useItem();
		void activate();
		void deactivate();*/
	private:
		std::shared_ptr<sf::Texture> _objectIcon;
		std::string _objectDescription, _objectName, _objectNameId;

	};
}