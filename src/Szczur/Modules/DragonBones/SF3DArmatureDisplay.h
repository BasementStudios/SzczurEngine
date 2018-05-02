/*
*********************************************************************
* File          : SF3DArmatureDisplay.h
* Project		: DragonBonesSFML
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
* License   	: MIT License
*********************************************************************
*/

#pragma once

#include <dragonBones/DragonBonesHeaders.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Rect.hpp>

#include "Szczur/Utility/SFML3D/Texture.hpp"
#include "Szczur/Utility/SFML3D/VertexArray.hpp"
#include "Szczur/Utility/SFML3D/RenderTarget.hpp"
#include "Szczur/Utility/SFML3D/RenderStates.hpp"

#include "SF3DEventDispatcher.h"

#include <glm/glm.hpp>

DRAGONBONES_NAMESPACE_BEGIN

class SF3DArmatureDisplay : public IArmatureProxy
{
protected:
	Armature*									_armature;
	SFMLEventDispatcher							_dispatcher;

	glm::vec3								_position;

public:
	SF3DArmatureDisplay();
	~SF3DArmatureDisplay();

	bool hasDBEventListener(const std::string& type) const override { return true; }
	void addDBEventListener(const std::string& type, const std::function<void(EventObject*)>& listener) override;
	void removeDBEventListener(const std::string& type, const std::function<void(EventObject*)>& listener) override;
	void dispatchDBEvent(const std::string& type, EventObject* value) override;

	void dbInit(Armature* armature) override;
	void dbClear() override;
	void dbUpdate() override;

	void dispose(bool disposeProxy) override;
	
	Armature* getArmature() const override { return _armature; }
	Animation* getAnimation() const override { return _armature->getAnimation(); }

	sf::FloatRect getBoundingBox() const;

	void setPosition(const glm::vec3& pos) { _position = pos; }
	auto& getPosition() { return _position; }

	void draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const;
};

DRAGONBONES_NAMESPACE_END
