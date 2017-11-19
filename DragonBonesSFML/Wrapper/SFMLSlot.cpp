#include "SFMLSlot.h"

#include <SFML\Graphics.hpp>

#include "SFMLMesh.h"
#include "SFMLArmatureDisplay.h"
#include "SFMLTextureAtlasData.h"
#include "SFMLTextureData.h"

DRAGONBONES_NAMESPACE_BEGIN

void SFMLSlot::_updateVisible()
{
	_renderDisplay->visible = _parent->getVisible();
}

void SFMLSlot::_updateBlendMode()
{
	return;

	if (_renderDisplay)
	{
		switch (_blendMode)
		{
			case BlendMode::Normal:
				_renderDisplay->blendMode = sf::BlendMode();
				break;

			case BlendMode::Add:
			{
				_renderDisplay->blendMode = sf::BlendAdd;
				break;
			}

			default:
				break;
		}
	}
	else if (_childArmature)
	{
		for (const auto slot : _childArmature->getSlots())
		{
			slot->_blendMode = _blendMode;
			slot->_updateBlendMode();
		}
	}
}

void SFMLSlot::_updateColor()
{
	if (_display != _meshDisplay && _renderDisplay->_spriteDisplay)
	{
		sf::Color helpColor;

		helpColor.a = _colorTransform.alphaMultiplier * 255.0f;
		helpColor.r = _colorTransform.redMultiplier * 255.0f;
		helpColor.g = _colorTransform.greenMultiplier * 255.0f;
		helpColor.b = _colorTransform.blueMultiplier * 255.0f;

		_renderDisplay->_spriteDisplay->setColor(helpColor);
	}
}

void SFMLSlot::_initDisplay(void* value)
{
	//const auto renderDisplay = static_cast<SFMLDisplay*>(value);
	//renderDisplay->retain();
}

void SFMLSlot::_disposeDisplay(void* value)
{
	//const auto renderDisplay = static_cast<SFMLDisplay*>(value);
	//delete renderDisplay;

}

void SFMLSlot::_onUpdateDisplay()
{
	_renderDisplay = std::unique_ptr<SFMLDisplay>(static_cast<SFMLDisplay*>(_display != nullptr ? _display : _rawDisplay));
}

void SFMLSlot::_addDisplay()
{
	//const auto container = static_cast<SFMLArmatureDisplay*>(_armature->getDisplay());
	//container->addChild(_renderDisplay);
}

void SFMLSlot::_replaceDisplay(void* value, bool isArmatureDisplay)
{
	/*const auto container = static_cast<SFMLArmatureDisplay*>(_armature->getDisplay());
	const auto prevDisplay = isArmatureDisplay ? static_cast<WrapperSprite*>(value) : static_cast<WrapperSprite*>(value);
	container->addChild(_renderDisplay, prevDisplay->getLocalZOrder());
	container->removeChild(prevDisplay);
	_textureScale = 1.0f;*/
}

void SFMLSlot::_removeDisplay()
{
	//_renderDisplay->removeFromParent();
}

void SFMLSlot::_updateZOrder()
{
	/*if (_renderDisplay->getLocalZOrder() == _zOrder)
	{
		return;
	}

	_renderDisplay->setLocalZOrder(_zOrder);*/
}

void SFMLSlot::_updateFrame()
{
	const auto meshData = _display == _meshDisplay ? _meshData : nullptr;
	auto currentTextureData = static_cast<SFMLTextureData*>(_textureData);

	if (_displayIndex >= 0 && _display != nullptr && currentTextureData != nullptr)
	{
		if (currentTextureData->Texture != nullptr)
		{
			if (meshData != nullptr) // SFMLMesh
			{
				const auto data = meshData->parent->parent->parent;
				const auto intArray = data->intArray;
				const auto floatArray = data->floatArray;
				const unsigned vertexCount = intArray[meshData->offset + (unsigned)BinaryOffset::MeshVertexCount];
				const unsigned triangleCount = intArray[meshData->offset + (unsigned)BinaryOffset::MeshTriangleCount];
				int vertexOffset = intArray[meshData->offset + (unsigned)BinaryOffset::MeshFloatOffset];

				if (vertexOffset < 0)
				{
					vertexOffset += 65536;
				}

				const unsigned uvOffset = vertexOffset + vertexCount * 2;

				const auto& region = currentTextureData->region;
				const auto& textureAtlasSize = currentTextureData->Texture->getSize();

				std::vector<std::shared_ptr<sf::Vertex>> vertices(vertexCount);
				//sf::VertexArray vertices(sf::PrimitiveType::TrianglesFan);

				std::vector<uint16_t> vertexIndices(triangleCount * 3);

				for (std::size_t i = 0, l = vertexCount * 2; i < l; i += 2)
				{
					const auto iH = i / 2;

					const auto x = floatArray[vertexOffset + i];
					const auto y = floatArray[vertexOffset + i + 1];
					auto u = floatArray[uvOffset + i];
					auto v = floatArray[uvOffset + i + 1];
					auto vertexData = std::make_shared<sf::Vertex>();
					//sf::Vertex vertexData;
					vertexData->position = { x, y };

					if (currentTextureData->rotated)
					{
						vertexData->texCoords.x = (region.x + (1.0f - v) * region.width);
						vertexData->texCoords.y = (region.y + u * region.height);
					}
					else
					{
						vertexData->texCoords.x = (region.x + u * region.width);
						vertexData->texCoords.y = (region.y + v * region.height);
					}

					vertexData->color = sf::Color::White;
					vertices[iH] = vertexData;
				}

				for (std::size_t i = 0; i < triangleCount * 3; ++i)
				{
					vertexIndices.push_back(intArray[meshData->offset + (unsigned)BinaryOffset::MeshVertexIndices + i]);
				}

				std::vector<sf::Vertex*> verticesDisplay;

				// sorting
				for (uint64_t i = 0; i < vertexIndices.size(); i++)
				{
					verticesDisplay.push_back(vertices[vertexIndices[i]].get());
				}

				_textureScale = 1.f;

				auto meshDisplay = new SFMLMesh();
				meshDisplay->texture = currentTextureData->Texture;
				meshDisplay->vertices = std::move(vertices);
				meshDisplay->verticesDisplay = std::move(verticesDisplay);

				_renderDisplay->_meshDisplay = std::unique_ptr<SFMLMesh>(meshDisplay);

			}
			else // Normal texture
			{

				const auto scale = currentTextureData->parent->scale * _armature->armatureData->scale;
				const auto height = (currentTextureData->rotated ? currentTextureData->region.width : currentTextureData->region.height) * scale;
				_pivotY -= height;
				_textureScale = scale; 

				auto spriteDisplay = std::make_unique<sf::Sprite>();
				spriteDisplay->setTexture(*currentTextureData->Texture);
				spriteDisplay->setTextureRect(currentTextureData->Rect);
				spriteDisplay->setOrigin({ 0.f, spriteDisplay->getLocalBounds().height });
				_renderDisplay->_spriteDisplay = std::move(spriteDisplay);
			}

			_visibleDirty = true;
			_blendModeDirty = true;
			_colorDirty = true;

			return;
		}
	}

	_renderDisplay->visible = false;
}

void SFMLSlot::_updateMesh()
{
	const auto hasFFD = !_ffdVertices.empty();
	const auto scale = _armature->armatureData->scale;
	const auto textureData = static_cast<SFMLTextureData*>(_textureData);
	const auto meshData = _meshData;
	const auto weightData = meshData->weight;
	const auto meshDisplay = _renderDisplay->_meshDisplay.get();
	auto& vertices = meshDisplay->vertices;

	if (!textureData || meshDisplay->texture != textureData->Texture)
	{
		return;
	}

	if (weightData != nullptr)
	{
		const auto data = meshData->parent->parent->parent;
		const auto intArray = data->intArray;
		const auto floatArray = data->floatArray;
		const auto vertexCount = (std::size_t)intArray[meshData->offset + (unsigned)BinaryOffset::MeshVertexCount];
		int weightFloatOffset = intArray[weightData->offset + (unsigned)BinaryOffset::WeigthFloatOffset];

		if (weightFloatOffset < 0)
		{
			weightFloatOffset += 65536;
		}

		for (
			std::size_t i = 0, iD = 0, iB = weightData->offset + (unsigned)BinaryOffset::WeigthBoneIndices + weightData->bones.size(), iV = (std::size_t)weightFloatOffset, iF = 0;
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

			auto& vertex = vertices[i];
			auto& vertexPosition = vertex->position;

			sf::Vector2f pos = static_cast<SFMLArmatureDisplay*>(_armature->getProxy())->getPosition();

			vertexPosition = { xG, yG };
			vertexPosition += pos;
		}
	}
	else if (hasFFD)
	{
		const auto data = meshData->parent->parent->parent;
		const auto intArray = data->intArray;
		const auto floatArray = data->floatArray;
		const auto vertexCount = (std::size_t)intArray[meshData->offset + (unsigned)BinaryOffset::MeshVertexCount];
		int vertexOffset = (std::size_t)intArray[meshData->offset + (unsigned)BinaryOffset::MeshFloatOffset];

		if (vertexOffset < 0)
		{
			vertexOffset += 65536;
		}

		for (std::size_t i = 0, l = vertexCount * 2; i < l; i += 2)
		{
			const auto iH = i / 2;
			const auto xG = floatArray[vertexOffset + i] * scale + _ffdVertices[i];
			const auto yG = floatArray[vertexOffset + i + 1] * scale + _ffdVertices[i + 1];

			auto& vertex = vertices[iH];
			auto& vertexPosition = vertex->position;

			sf::Vector2f pos = static_cast<SFMLArmatureDisplay*>(_armature->getProxy())->getPosition();

			vertexPosition = { xG, yG };
			vertexPosition += pos;
		}
	}
}

void SFMLSlot::_updateTransform(bool isSkinnedMesh)
{
	if (isSkinnedMesh)
	{
		auto matrix = sf::Transform(1.f, 0.f, 0.f,
									0.f, 1.f, 0.f,
									0.f, 0.f, 1.f);

		_renderDisplay->matrix = matrix;
	}
	else
	{
		sf::Vector2f pos;

		if (_renderDisplay.get() == _rawDisplay || _renderDisplay.get() == _meshDisplay)
		{
			globalTransformMatrix.ty = globalTransformMatrix.ty;

			pos.x = globalTransformMatrix.tx - (globalTransformMatrix.a * _pivotX + globalTransformMatrix.c * _pivotY);
			pos.y = globalTransformMatrix.ty - (globalTransformMatrix.b * _pivotX + globalTransformMatrix.d * _pivotY);
		}
		else if (_childArmature)
		{
			globalTransformMatrix.ty = globalTransformMatrix.ty;

			pos.x = globalTransformMatrix.tx;
			pos.y = globalTransformMatrix.ty;
		}
		else
		{
			sf::Vector2f anchorPoint = { 1.f, 1.f };
			pos.x = globalTransformMatrix.tx - (globalTransformMatrix.a * anchorPoint.x - globalTransformMatrix.c * anchorPoint.y);
			pos.y = globalTransformMatrix.ty - (globalTransformMatrix.b * anchorPoint.x - globalTransformMatrix.d * anchorPoint.y);
		}

		pos += static_cast<SFMLArmatureDisplay*>(_armature->getProxy())->getPosition();

		auto matrix = sf::Transform(globalTransformMatrix.a * _textureScale, -globalTransformMatrix.c * _textureScale, pos.x * _textureScale,
									globalTransformMatrix.b * _textureScale, -globalTransformMatrix.d * _textureScale, pos.y * _textureScale,
									0.f, 0.f, 1.f);
					
		matrix.scale(1.f, -1.f);

		_renderDisplay->matrix = matrix;
	}
}

void SFMLSlot::_onClear()
{
	Slot::_onClear();

	_textureScale = 1.0f;

	if (_textureData)
	{
		delete _textureData;
		_textureData = nullptr;
	}
}

DRAGONBONES_NAMESPACE_END
