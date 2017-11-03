#include "WrapperSlot.h"

#include <SFML\Graphics.hpp>

#include "WrapperArmatureDisplay.h"
#include "WrapperTextureAtlasData.h"
#include "WrapperTextureData.h"

void WrapperSlot::_updateVisible()
{
	//_renderDisplay->setVisible(_parent->getVisible());
}

void WrapperSlot::_updateBlendMode()
{
	//WrapperSprite* spriteDisplay = dynamic_cast<WrapperSprite*>(_renderDisplay);
	//if (spriteDisplay)
	//{
	//	switch (_blendMode)
	//	{
	//		case dragonBones::BlendMode::Normal:
	//			//spriteDisplay->setBlendFunc(cocos2d::BlendFunc::DISABLE);
	//			break;

	//		case dragonBones::BlendMode::Add:
	//		{
	//			const auto texture = spriteDisplay->getTexture();
	//			if (texture /*&& texture->hasPremultipliedAlpha()*/)
	//			{
	//				/*cocos2d::BlendFunc blendFunc = { GL_ONE, GL_ONE };
	//				spriteDisplay->setBlendFunc(blendFunc);*/

	//			}
	//			else
	//			{
	//				//spriteDisplay->setBlendFunc(cocos2d::BlendFunc::ADDITIVE);
	//			}
	//			break;
	//		}

	//		default:
	//			break;
	//	}
	//}
	//else if (_childArmature)
	//{
	//	for (const auto slot : _childArmature->getSlots())
	//	{
	//		slot->_blendMode = _blendMode;
	//		slot->_updateBlendMode();
	//	}
	//}
}

void WrapperSlot::_updateColor()
{
	/*_renderDisplay->setOpacity(_colorTransform.alphaMultiplier * 255.0f);

	static cocos2d::Color3B helpColor;
	helpColor.r = _colorTransform.redMultiplier * 255.0f;
	helpColor.g = _colorTransform.greenMultiplier * 255.0f;
	helpColor.b = _colorTransform.blueMultiplier * 255.0f;

	_renderDisplay->setColor(helpColor);*/
}

void WrapperSlot::_initDisplay(void* value)
{
	//const auto renderDisplay = static_cast<sf::Sprite*>(value);
	//renderDisplay->retain();
}

void WrapperSlot::_disposeDisplay(void* value)
{
	//const auto renderDisplay = static_cast<WrapperSprite*>(value);
	//delete renderDisplay;

}

void WrapperSlot::_onUpdateDisplay()
{
	// musi byæ
	_renderDisplay = static_cast<WrapperSprite*>(_display != nullptr ? _display : _rawDisplay);
}

void WrapperSlot::_addDisplay()
{
	//const auto container = static_cast<WrapperArmatureDisplay*>(_armature->getDisplay());
	//container->addChild(_renderDisplay);
}

void WrapperSlot::_replaceDisplay(void* value, bool isArmatureDisplay)
{
	/*const auto container = static_cast<WrapperArmatureDisplay*>(_armature->getDisplay());
	const auto prevDisplay = isArmatureDisplay ? static_cast<WrapperSprite*>(value) : static_cast<WrapperSprite*>(value);
	container->addChild(_renderDisplay, prevDisplay->getLocalZOrder());
	container->removeChild(prevDisplay);
	_textureScale = 1.0f;*/
}

void WrapperSlot::_removeDisplay()
{
	//_renderDisplay->removeFromParent();
}

void WrapperSlot::_updateZOrder()
{
	/*if (_renderDisplay->getLocalZOrder() == _zOrder)
	{
		return;
	}

	_renderDisplay->setLocalZOrder(_zOrder);*/
}

void WrapperSlot::_updateFrame()
{
	const auto meshData = _display == _meshDisplay ? _meshData : nullptr;
	auto currentTextureData = static_cast<WrapperTextureData*>(_textureData);
	//const auto frameDisplay = static_cast<WrapperSprite*>(_renderDisplay); // In cocos2dx render meshDisplay and frameDisplay are the same display

	if (_displayIndex >= 0 && _display != nullptr && currentTextureData != nullptr)
	{
		if (currentTextureData->Sprite != nullptr)
		{
			if (meshData == nullptr) // Mesh.
			{
				const auto scale = currentTextureData->parent->scale * _armature->armatureData->scale;
				const auto height = (currentTextureData->rotated ? currentTextureData->region.width : currentTextureData->region.height) * scale;
				_pivotY -= height;
				_textureScale = scale; // cocos2d::Director::getInstance()->getContentScaleFactor();

				 //frameDisplay->setSpriteFrame(currentTextureData->spriteFrame); // polygonInfo will be override

				_renderDisplay->sprite->setTexture(*currentTextureData->Sprite->getTexture());
				_renderDisplay->sprite->setTextureRect(currentTextureData->Sprite->getTextureRect());
			}

			_visibleDirty = true;
			_blendModeDirty = true; // Relpace texture will override blendMode and color.
			_colorDirty = true;

			return;
		}
	}

	_renderDisplay->sprite->setTexture(sf::Texture());
	_renderDisplay->sprite->setTextureRect(sf::IntRect());
	_renderDisplay->sprite->setPosition(0.0f, 0.0f);
}

void WrapperSlot::_updateMesh()
{
	/*
	const auto hasFFD = !_ffdVertices.empty();
	const auto scale = _armature->armatureData->scale;
	const auto textureData = static_cast<WrapperTextureData*>(_textureData);
	const auto meshData = _meshData;
	const auto weightData = meshData->weight;
	const auto meshDisplay = static_cast<WrapperSprite*>(_renderDisplay);
	//const auto vertices = meshDisplay->getPolygonInfoModify().triangles.verts;
	sf::FloatRect boundsRect(999999.0f, 999999.0f, -999999.0f, -999999.0f);

	if (!textureData || meshDisplay != textureData->Sprite)
	{
		return;
	}

	if (weightData != nullptr)
	{
		const auto data = meshData->parent->parent->parent;
		const auto intArray = data->intArray;
		const auto floatArray = data->floatArray;
		const auto vertexCount = (std::size_t)intArray[meshData->offset + (unsigned)dragonBones::BinaryOffset::MeshVertexCount];
		int weightFloatOffset = intArray[weightData->offset + (unsigned)dragonBones::BinaryOffset::WeigthFloatOffset];

		if (weightFloatOffset < 0)
		{
			weightFloatOffset += 65536; // Fixed out of bouds bug. 
		}

		for (
			std::size_t i = 0, iD = 0, iB = weightData->offset + (unsigned)dragonBones::BinaryOffset::WeigthBoneIndices + weightData->bones.size(), iV = (std::size_t)weightFloatOffset, iF = 0;
			i < vertexCount;
			++i
			)
		{
			const auto boneCount = (std::size_t)intArray[iB++];
			auto xG = 0.0f, yG = 0.0f;
			for (std::size_t j = 0; j < boneCount; ++j)
			{
				const auto boneIndex = (unsigned)intArray[iB++];
				const auto bone = _meshBones[boneIndex];
				if (bone != nullptr)
				{
					const auto& matrix = bone->globalTransformMatrix;
					const auto weight = floatArray[iV++];
					auto xL = floatArray[iV++] * scale;
					auto yL = floatArray[iV++] * scale;

					if (hasFFD)
					{
						xL += _ffdVertices[iF++];
						yL += _ffdVertices[iF++];
					}

					xG += (matrix.a * xL + matrix.c * yL + matrix.tx) * weight;
					yG += (matrix.b * xL + matrix.d * yL + matrix.ty) * weight;
				}
			}

			//auto& vertex = vertices[i];
			//auto& vertexPosition = vertex.vertices;

			//vertexPosition.set(xG, -yG, 0.0f);

			if (boundsRect.left > xG)
			{			  
				boundsRect.left = xG;
			}

			if (boundsRect.width < xG)
			{
				boundsRect.width = xG;
			}

			if (boundsRect.top > -yG)
			{			   
				boundsRect.top = -yG;
			}

			if (boundsRect.height < -yG)
			{
				boundsRect.height = -yG;
			}
		}
	}
	else if (hasFFD)
	{
		const auto data = meshData->parent->parent->parent;
		const auto intArray = data->intArray;
		const auto floatArray = data->floatArray;
		const auto vertexCount = (std::size_t)intArray[meshData->offset + (unsigned)dragonBones::BinaryOffset::MeshVertexCount];
		int vertexOffset = (std::size_t)intArray[meshData->offset + (unsigned)dragonBones::BinaryOffset::MeshFloatOffset];

		if (vertexOffset < 0)
		{
			vertexOffset += 65536; // Fixed out of bouds bug. 
		}

		for (std::size_t i = 0, l = vertexCount * 2; i < l; i += 2)
		{
			const auto iH = i / 2;
			const auto xG = floatArray[vertexOffset + i] * scale + _ffdVertices[i];
			const auto yG = floatArray[vertexOffset + i + 1] * scale + _ffdVertices[i + 1];

			//auto& vertex = vertices[iH];
			//auto& vertexPosition = vertex.vertices;

			// vertexPosition.set(xG, -yG, 0.0f);

			if (boundsRect.left > xG)
			{			 
				boundsRect.left = xG;
			}

			if (boundsRect.width < xG)
			{
				boundsRect.width = xG;
			}

			if (boundsRect.top > -yG)
			{			   
				boundsRect.top = -yG;
			}

			if (boundsRect.height < -yG)
			{
				boundsRect.height = -yG;
			}
		}
	}

	boundsRect.width -= boundsRect.left;
	boundsRect.height -= boundsRect.top;

	/*auto polygonInfo = meshDisplay->getPolygonInfo();
	const auto& transform = meshDisplay->getTransform(); // Backup transform. (Set rect and polygon will override transform).

#if COCOS2D_VERSION >= 0x00031400
	polygonInfo.setRect(boundsRect);
#else
	polygonInfo.rect = boundsRect; // Copy
	meshDisplay->setContentSize(boundsRect.size);
#endif
	meshDisplay->setPolygonInfo(polygonInfo);
	meshDisplay->setNodeToParentTransform(transform);*/
}

void WrapperSlot::_updateTransform(bool isSkinnedMesh)
{
	/*transform.m[0] = globalTransformMatrix.a;
	transform.m[1] = globalTransformMatrix.b;
	transform.m[4] = -globalTransformMatrix.c;
	transform.m[5] = -globalTransformMatrix.d;*/

	if (_renderDisplay == _rawDisplay || _renderDisplay == _meshDisplay)
	{
		if (_textureScale != 1.0f)
		{
			/*transform.m[0] *= _textureScale;
			transform.m[1] *= _textureScale;
			transform.m[4] *= _textureScale;
			transform.m[5] *= _textureScale;*/
		}


		sf::Vector2f pos;

		pos.x = globalTransformMatrix.tx - (globalTransformMatrix.a * _pivotX + globalTransformMatrix.c * _pivotY);
		pos.y = globalTransformMatrix.ty - (globalTransformMatrix.b * _pivotX + globalTransformMatrix.d * _pivotY);

		//pos.x = globalTransformMatrix.tx - (globalTransformMatrix.d * _pivotX + globalTransformMatrix.d * _pivotY);
	    //pos.y = -globalTransformMatrix.ty - (globalTransformMatrix.c * _pivotX + globalTransformMatrix.a * _pivotY);

		static const sf::Vector2f offset(255.f, 255.f);

		_renderDisplay->matrix = sf::Transform(globalTransformMatrix.a,	-globalTransformMatrix.c, pos.x,
											   globalTransformMatrix.b, -globalTransformMatrix.d, pos.y,
											   0.f, 0.f, 0.f);

		/*_renderDisplay->matrix = sf::Transform(-globalTransformMatrix.d, globalTransformMatrix.b, pos.x,
											   -globalTransformMatrix.c, globalTransformMatrix.a, pos.y,
											   0.f, 0.f, 0.f);*/
	}
}
