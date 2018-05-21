#include "BattleTexture.hpp"

namespace rat {

bool BattleTexture::load(const std::string& filePath, int start_x, int start_y, int frame_w, int frame_h, int framesInRow) {
	this->filePath = filePath;
	this->start_x = start_x;
	this->start_y = start_y;
	this->frame_w = frame_w;
	this->frame_h = frame_h;
	this->framesInRow = framesInRow;
	isGood = texture.loadFromFile(filePath);
}

bool BattleTexture::good() const {
	return isGood;
}

sf::Sprite BattleTexture::getSprite(int frame) const {
	sf::Sprite ret;
	int x = frame%framesInRow;
	int y = frame/framesInRow;
	if(isGood) {
		ret.setTexture(texture);
	}
	ret.setTextureRect({start_x+frame_w*x+x, start_y+frame_h*y+y, frame_w, frame_h});
	return ret;
}

}