#include "MiniFrame.hpp"

#include <SFML/Graphics.hpp>

namespace rat {

	MiniFrame::MiniFrame(sf::Vector2f& targetPos) {
		pos = &targetPos;
	}
	
	bool MiniFrame::isHovered(const sf::Vector2f& point) {
		if(clickable == false) return false;
		return point.x>pos->x-size.x/2.f && point.y>pos->y-size.y/2.f && 
			point.x<pos->x+size.x-size.x/2.f && point.y<pos->y+size.y-size.y/2.f;
	}
	
	void MiniFrame::setClickable(bool flag) {
		clickable = flag;
	}
	
	bool MiniFrame::getClickable() {
		return clickable;
	}
	
	const sf::RectangleShape& MiniFrame::getShape(int state) {
		shape.setOrigin({size.x/2.f, size.y/2.f});
		shape.setSize({size.x+4, size.y+4}); 
		shape.setPosition({pos->x-2, pos->y-2}); 
		shape.setFillColor({0,0,0,0});
		
		if(clickable == false) {			
			shape.setOutlineThickness(1); 
			shape.setOutlineColor({30,30,20,255}); 
			return shape;
		}
		
		if(state == 0) { 
			shape.setOutlineThickness(1); 
			shape.setOutlineColor({90,70,50,255}); 
			return shape;
		} 
		else if(state == 1) { 
			shape.setOutlineThickness(1); 
			shape.setOutlineColor({140,140,140,255}); 
			return shape;
		} 
		else if(state == 2) { 
			shape.setOutlineThickness(1); 
			shape.setOutlineColor({210,140,140,255}); 
			return shape;
		} 
		else if(state == 3) { 
			shape.setOutlineThickness(2); 
			shape.setOutlineColor({210,140,140,255}); 
			return shape;
		} 
		return shape;
	}
}