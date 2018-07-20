#include "BattleSpellIndicatorManager.hpp"

#include "BattleScene.hpp"

#include <Szczur/Modules/Script/Script.hpp>

namespace rat
{

BattleSpellIndicatorManager::BattleSpellIndicatorManager()
{
	// Circle indicator
	_circleTexture[0].loadFromFile("Assets/Battles/indicators/circle_indicator.png");	
	_circleTexture[1].loadFromFile("Assets/Battles/indicators/circle_indicator_red.png");	
	_circleSprite.setTexture(_circleTexture[0]);
	_circleSprite.setOrigin({1024.f/2.f, 1024.f/2.f, -6.f});
	_circleSprite.setRotation({-90.f, 0.f, 0.f});
	_circleFactor = 1.f/1024.f;

	// Line indicator

	// > 1
	_lineTexture[0].loadFromFile("Assets/Battles/indicators/line_indicator.png");	
	_lineTexture[1].loadFromFile("Assets/Battles/indicators/line_indicator_red.png");	
	_lineSprite.setTexture(_lineTexture[0]);
	_lineSprite.setOrigin({0.f, 440.f/2.f, -6.f});	
	_lineSprite.setRotation({-90.f, 0.f, 0.f});

	// > 2
	_lineTexture2[0].loadFromFile("Assets/Battles/indicators/line_indicator_2.png");	
	_lineTexture2[1].loadFromFile("Assets/Battles/indicators/line_indicator_2_red.png");	
	_lineSprite2.setTexture(_lineTexture2[0]);
	_lineSprite2.setOrigin({0.f, 440.f/2.f, -6.f});	
	_lineSprite2.setRotation({-90.f, 0.f, 0.f});

	// > Factors
	_lineFactorX = 1.f/1024.f;
	_lineFactorY = 1.f/440.f;

	// Hp bar indicator

	// > Bar back
	_hpBarBackTexture[0].loadFromFile("Assets/Battles/indicators/bars/hp_bar/hp_bar_back_1.png");	
	_hpBarBackTexture[1].loadFromFile("Assets/Battles/indicators/bars/hp_bar/hp_bar_back_2.png");	
	_hpBarBackTexture[2].loadFromFile("Assets/Battles/indicators/bars/hp_bar/hp_bar_back_3.png");

	// > Bar
	_hpBarTexture[0].loadFromFile("Assets/Battles/indicators/bars/hp_bar/hp_bar_1.png");	
	_hpBarTexture[1].loadFromFile("Assets/Battles/indicators/bars/hp_bar/hp_bar_2.png");	
	_hpBarTexture[2].loadFromFile("Assets/Battles/indicators/bars/hp_bar/hp_bar_3.png");	

	// > Factors
	_hpBarFactor = 0.7f;

	// Time bar indicator

	// > Bar
	_timeBarTexture[0].loadFromFile("Assets/Battles/indicators/bars/time_bar/time_bar_1.png");
	_timeBarTexture[1].loadFromFile("Assets/Battles/indicators/bars/time_bar/time_bar_2.png");
	_timeBarTexture[2].loadFromFile("Assets/Battles/indicators/bars/time_bar/time_bar_3.png");

	// > Bar back
	_timeBarBackTexture[0].loadFromFile("Assets/Battles/indicators/bars/time_bar/time_bar_back_1.png");
	_timeBarBackTexture[1].loadFromFile("Assets/Battles/indicators/bars/time_bar/time_bar_back_2.png");
	_timeBarBackTexture[2].loadFromFile("Assets/Battles/indicators/bars/time_bar/time_bar_back_3.png");

	// > Bar full
	_timeBarFullTexture[0].loadFromFile("Assets/Battles/indicators/bars/time_bar/time_bar_full_1.png");
	_timeBarFullTexture[1].loadFromFile("Assets/Battles/indicators/bars/time_bar/time_bar_full_2.png");
	_timeBarFullTexture[2].loadFromFile("Assets/Battles/indicators/bars/time_bar/time_bar_full_3.png");

	// > Factors
	_timeBarFactor = 0.5f;

	// Status bar indicator

	_statusBarTexture[0].loadFromFile("Assets/Battles/indicators/bars/status_bar/status_1.png");;
	_statusBarTexture[1].loadFromFile("Assets/Battles/indicators/bars/status_bar/status_2.png");;
	_statusBarFactor = 1.2f;
}

void BattleSpellIndicatorManager::setRenderTarget(sf3d::RenderTarget& canvas)
{
	_canvas = &canvas;
}

void BattleSpellIndicatorManager::setHeight(float height)
{
	_height = height;
}

void BattleSpellIndicatorManager::setBattleScene(BattleScene* scene)
{
	_scene = scene;
}

void BattleSpellIndicatorManager::renderCircleIndicator(int index, glm::vec2 pos, float radius, float deltaY)
{
	float size = radius*2.f*_circleFactor;
	_circleSprite.setTexture(_circleTexture[index]);
	_circleSprite.setScale(glm::vec3(size, size, 1.f));
	_circleSprite.setPosition(glm::vec3(pos.x, _scene->getPosition().y + deltaY, pos.y));

	_canvas->draw(_circleSprite);
}

void BattleSpellIndicatorManager::renderLineIndicator(int index, glm::vec2 pos, float width, float height, float angle, float deltaY)
{
	float sizeX = width*_lineFactorX;
	float sizeY = height*_lineFactorY;

	_lineSprite.setTexture(_lineTexture[index]);
	_lineSprite.setScale(glm::vec3(sizeX, sizeY, 1.f));
	_lineSprite.setPosition(glm::vec3(pos.x, _scene->getPosition().y + deltaY, pos.y));
	_lineSprite.setRotation({-90.f, 0.f, -glm::degrees(angle)});

	_canvas->draw(_lineSprite);
}

void BattleSpellIndicatorManager::renderLineIndicator2(int index, glm::vec2 pos, float width, float height, float angle, float deltaY)
{
	float sizeX = width*_lineFactorX;
	float sizeY = height*_lineFactorY;

	_lineSprite2.setTexture(_lineTexture2[index]);
	_lineSprite2.setScale(glm::vec3(sizeX, sizeY, 1.f));
	_lineSprite2.setPosition(glm::vec3(pos.x, _scene->getPosition().y + deltaY, pos.y));
	_lineSprite2.setRotation({-90.f, 0.f, -glm::degrees(angle)});

	_canvas->draw(_lineSprite2);
}

void BattleSpellIndicatorManager::renderHpBarIndicator(glm::vec2 pos, float height, float hp, float maxHp)
{
	int index = 0;
	if(maxHp>=200) index = 2;
	else if(maxHp>=100) index = 1;

	renderHpBarIndicator(pos, height, hp, maxHp, index);
}

void BattleSpellIndicatorManager::renderHpBarIndicator(glm::vec2 pos, float height, float hp, float maxHp, int sizeIndex)
{
	_hpBarSprite.setScale({_hpBarFactor, _hpBarFactor, 1.f});

	// Hp bar back
	{
		const sf3d::Texture& tex = _hpBarBackTexture[sizeIndex];
		_hpBarSprite.setTexture(tex);
		_hpBarSprite.setOrigin({ tex.getSize().x / 2.f, tex.getSize().y / 2.f, 0.f });
		_hpBarSprite.setPosition({ pos.x, _scene->getPosition().y + height, pos.y });
		_canvas->draw(_hpBarSprite);
	}

	// Hp bar
	{
		const sf3d::Texture& tex = _hpBarTexture[sizeIndex];
		_hpBarSprite.setTexture(tex);
		_hpBarSprite.setOrigin({ tex.getSize().x / 2.f, tex.getSize().y / 2.f, -1.f });
		_hpBarSprite.setPosition({ pos.x, _scene->getPosition().y + height, pos.y });
		_hpBarSprite.setTextureRect({ 0.f, 0.f }, { tex.getSize().x*hp / maxHp, tex.getSize().y });
		_canvas->draw(_hpBarSprite);
	}
}

void BattleSpellIndicatorManager::renderTimeBarIndicator(glm::vec2 pos, float height, float time, float maxTime)
{	
	int index = 0;
	if(maxTime>=200) index = 2;
	else if(maxTime>=100) index = 1;

	renderTimeBarIndicator(pos, height, time, maxTime, index);
}

void BattleSpellIndicatorManager::renderTimeBarIndicator(glm::vec2 pos, float height, float time, float maxTime, int sizeIndex)
{
	_timeBarSprite.setScale({_timeBarFactor, _timeBarFactor, 1.f});

	// Time bar back
	{
		const sf3d::Texture& tex = _timeBarBackTexture[sizeIndex];
		_timeBarSprite.setTexture(tex);
		_timeBarSprite.setOrigin({ tex.getSize().x / 2.f, tex.getSize().y / 2.f, 2.f });
		_timeBarSprite.setPosition({ pos.x, _scene->getPosition().y + height, pos.y });
		_canvas->draw(_timeBarSprite); 
	}

	if(time<maxTime) {

		// Time bar
		const sf3d::Texture& tex = _timeBarTexture[sizeIndex];
		_timeBarSprite.setTexture(tex);
		_timeBarSprite.setOrigin({tex.getSize().x/2.f, tex.getSize().y/2.f, 1.f});
		_timeBarSprite.setPosition({pos.x, _scene->getPosition().y + height, pos.y});
		_timeBarSprite.setTextureRect({0.f, 0.f}, {tex.getSize().x*time/maxTime, tex.getSize().y});
		_canvas->draw(_timeBarSprite);
	}
	else {

		// Time bar full
		const sf3d::Texture& tex = _timeBarFullTexture[sizeIndex];
		_timeBarSprite.setTexture(tex);
		_timeBarSprite.setOrigin({tex.getSize().x/2.f, tex.getSize().y/2.f, 1.f});
		_timeBarSprite.setPosition({pos.x, _scene->getPosition().y + height, pos.y});
		_canvas->draw(_timeBarSprite);
	}
}

void BattleSpellIndicatorManager::renderStatusBarIndicator(glm::vec2 pos, float height, int positionIndex, int statusIndex)
{
	_statusBarSprite.setScale({_statusBarFactor, _statusBarFactor, 1.f});

	sf3d::Texture& tex = _statusBarTexture [statusIndex];
	_statusBarSprite.setTexture(tex);
	_statusBarSprite.setOrigin({tex.getSize().x/2.f, tex.getSize().y/2.f, -2.f});

	_statusBarSprite.setPosition({pos.x + positionIndex*(tex.getSize().x*_statusBarFactor+5.f), _scene->getPosition().y + height, pos.y});
	_canvas->draw(_statusBarSprite);
}


void BattleSpellIndicatorManager::renderStatusBarIndicator(glm::vec2 pos, float height, const std::vector<int>& statuses)
{
	int i = 0;
	for(auto& status : statuses) {
		renderStatusBarIndicator(pos, height, i, status);
		++i;
	}
}

void BattleSpellIndicatorManager::initScript(Script& script)
{
	auto object = script.newClass<BattleSpellIndicatorManager>("BattleSpellIndicatorManager", "Battle");

	object.set("setHeight", &BattleSpellIndicatorManager::setHeight);
	object.set("renderCircleIndicator", &BattleSpellIndicatorManager::renderCircleIndicator);
	object.set("renderLineIndicator", &BattleSpellIndicatorManager::renderLineIndicator);
	object.set("renderLineIndicator2", &BattleSpellIndicatorManager::renderLineIndicator2);

	object.init();
}

}
