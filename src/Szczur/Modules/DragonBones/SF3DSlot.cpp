/** @file SF3DSlot.cpp
** @author Piotr Krupa (piotrkrupa06@gmail.com)
** @author Patryk (PsychoX) Ludwikowski <psychoxivi@gmail.com>
**/

#include "SF3DSlot.hpp"

#include <SFML/Graphics.hpp>

#include "SF3DArmatureDisplay.hpp"
#include "SF3DTextureAtlasData.hpp"
#include "SF3DTextureData.hpp"

#include "Szczur/Utility/SFML3D/VertexArray.hpp"
#include "Szczur/Utility/SFML3D/Vertex.hpp"

DRAGONBONES_NAMESPACE_BEGIN

void SF3DSlot::_updateVisible()
{
	_renderDisplay->visible = _parent->getVisible();
}

void SF3DSlot::_updateBlendMode()
{
	/*if (_renderDisplay)
	{
		switch (_blendMode)
		{
			case BlendMode::Normal:
				_renderDisplay->blendMode = sf::BlendMode();
				break;
			case BlendMode::Add:
				_renderDisplay->blendMode = sf::BlendAdd;
				break;
			case BlendMode::Multiply:
				_renderDisplay->blendMode = sf::BlendMultiply;
				break;
			default:
				_renderDisplay->blendMode = sf::BlendMode();
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
	}**/
}

void SF3DSlot::_updateColor()
{
	if (_renderDisplay)
	{
		_renderDisplay->setColor(_colorTransform);
	}
}

void SF3DSlot::_initDisplay(void* value, bool isRetain)
{
}

void SF3DSlot::_disposeDisplay(void* value, bool isRelease)
{
}

void SF3DSlot::_onUpdateDisplay()
{
	_renderDisplay = std::unique_ptr<SF3DDisplay>(static_cast<SF3DDisplay*>(_display != nullptr ? _display : _rawDisplay));
}

void SF3DSlot::_addDisplay()
{
}

void SF3DSlot::_replaceDisplay(void* value, bool isArmatureDisplay)
{
}

void SF3DSlot::_removeDisplay()
{
}

void SF3DSlot::_updateZOrder()
{
}

void SF3DSlot::_updateFrame()
{
	const auto currentVerticesData = (_deformVertices != nullptr && _display == _meshDisplay) ? _deformVertices->verticesData : nullptr;
	auto currentTextureData = static_cast<SF3DTextureData*>(_textureData);

	if (_displayIndex >= 0 && _display != nullptr && currentTextureData != nullptr)
	{
		if (currentTextureData->texture != nullptr)
		{
			if (currentVerticesData != nullptr) // Mesh
			{
				const auto data = currentVerticesData->data;
				const auto intArray = data->intArray;
				const auto floatArray = data->floatArray;
				const unsigned vertexCount = intArray[currentVerticesData->offset + (unsigned)BinaryOffset::MeshVertexCount];
				const unsigned triangleCount = intArray[currentVerticesData->offset + (unsigned)BinaryOffset::MeshTriangleCount];
				int vertexOffset = intArray[currentVerticesData->offset + (unsigned)BinaryOffset::MeshFloatOffset];

				if (vertexOffset < 0)
				{
					vertexOffset += 65536;
				}

				const unsigned uvOffset = vertexOffset + vertexCount * 2;

				const auto& region = currentTextureData->region;

				glm::vec2 texSize = currentTextureData->texture->getSize();

				std::vector<sf3d::Vertex> vertices(vertexCount);

				std::vector<std::vector<int>> verticesInTriagles;

				std::vector<uint16_t> vertexIndices(triangleCount * 3);

				for (std::size_t i = 0, l = vertexCount * 2; i < l; i += 2)
				{
					const auto iH = i / 2;

					const auto x = floatArray[vertexOffset + i];
					const auto y = floatArray[vertexOffset + i + 1];
					auto u = floatArray[uvOffset + i];
					auto v = floatArray[uvOffset + i + 1];

					sf3d::Vertex vertexData;

					vertexData.position = { x, y, 1.f };

					if (currentTextureData->rotated)
					{
						vertexData.texCoord.x = (region.x + (1.0f - v) * region.width) / texSize.x;
						vertexData.texCoord.y = (region.y + u * region.height) / texSize.y;
					}
					else
					{
						vertexData.texCoord.x = (region.x + u * region.width) / texSize.x;
						vertexData.texCoord.y = (region.y + v * region.height) / texSize.y;
					}

					vertexData.color = { 1.f, 1.f, 1.f, 1.f };

					vertices[iH] = vertexData;
				}

				for (std::size_t i = 0; i < triangleCount * 3; ++i)
				{
					vertexIndices.push_back(intArray[currentVerticesData->offset + (unsigned)BinaryOffset::MeshVertexIndices + i]);
				}

				_renderDisplay->verticesDisplay.resize(vertexIndices.size());

				verticesInTriagles.resize(vertices.size());

				// sorting
				for (unsigned int i = 0; i < vertexIndices.size(); i++)
				{
					verticesInTriagles[vertexIndices[i]].push_back(i);
					_renderDisplay->verticesDisplay.set(i, vertices[vertexIndices[i]]);
				}

				_textureScale = 1.f;

				_renderDisplay->texture = currentTextureData->texture;
				_renderDisplay->verticesInTriagles = std::move(verticesInTriagles);
				_renderDisplay->verticesDisplay.setPrimitveType(GL_TRIANGLES);

				const auto isSkinned = currentVerticesData->weight != nullptr;
				if (isSkinned)
				{
					_identityTransform();
				}
			}
			else // Normal texture
			{
				// Apply texture scaling
				_textureScale = currentTextureData->parent->scale * _armature->_armatureData->scale; // @warn no scale on mesh?

				_renderDisplay->texture = currentTextureData->texture;
				_renderDisplay->verticesDisplay.resize(4);
				_renderDisplay->verticesInTriagles.resize(0);
				_renderDisplay->primitiveType = GL_TRIANGLE_FAN;

				_renderDisplay->verticesDisplay.setPrimitveType(GL_TRIANGLE_FAN);
				// Setup verticles
				{
					const auto& region = currentTextureData->region;

					auto& verts = _renderDisplay->verticesDisplay;
					auto size = _renderDisplay->texture->getSize();

					float sizeX = static_cast<float>(size.x);
					float sizeY = static_cast<float>(size.y);

					float texX = region.x / sizeX;
					float texY = region.y / sizeY;
					float texWidth = region.width / sizeX + texX;
					float texHeight = region.height / sizeY + texY;


					verts.set(0u, sf3d::Vertex {
						{ 0.f, 0.f, 0.f },
						{ 1.f, 1.f, 1.f, 1.f },
						{ texX, texY }
					});
					verts.set(1u, sf3d::Vertex{
						{ region.width, 0.f, 0.f },
						{ 1.f, 1.f, 1.f, 1.f },
						{ texWidth, texY }
					});
					verts.set(2u, sf3d::Vertex{
						{ region.width, region.height, 0.f },
						{ 1.f, 1.f, 1.f, 1.f },
						{ texWidth, texHeight }
					 });

					verts.set(3u, sf3d::Vertex{
						{ 0.f, region.height, 0.f },
						{ 1.f, 1.f, 1.f, 1.f },
						{ texX, texHeight }
					});
				}
			}

			_visibleDirty = true;
			_blendModeDirty = true;
			_colorDirty = true;

			return;
		}
	}

	_renderDisplay->visible = false;
}

void SF3DSlot::_updateMesh()
{
	const auto& deformVertices = _deformVertices->vertices;
	const auto& deformBones = _deformVertices->bones;
	const auto& verticesData = _deformVertices->verticesData;
	const auto& weightData = verticesData->weight;

	const auto hasFFD = !deformVertices.empty();

	// Weightness
	if (weightData != nullptr)
	{
		const auto data = verticesData->data;
		const int16_t* intArray = data->intArray;
		const float* floatArray = data->floatArray;

		const std::size_t vertexCount = intArray[verticesData->offset + (std::size_t)BinaryOffset::MeshVertexCount];
		const std::size_t weightFloatOffset = intArray[weightData->offset + (std::size_t)BinaryOffset::WeigthFloatOffset];

		const auto scale = _armature->_armatureData->scale; // @warn what with _textureScale?

		std::size_t deformWeightsIndex = (std::size_t)weightFloatOffset;
		std::size_t deformVertexIndex = 0;
		std::size_t deformBonesIndex = weightData->offset + (std::size_t)BinaryOffset::WeigthBoneIndices + deformBones.size();

		for (std::size_t i = 0; i < vertexCount; ++i)
		{
			float xG = 0.0f;
			float yG = 0.0f;

			// Calculate new position taking into account bone weights
			const std::size_t boneCount = intArray[deformBonesIndex++];
			for (std::size_t j = 0; j < boneCount; ++j)
			{
				const std::size_t boneIndex = intArray[deformBonesIndex++];
				const auto bone = deformBones[boneIndex];

				if (bone != nullptr)
				{
					const auto& matrix = bone->globalTransformMatrix;

					const auto weight = floatArray[deformWeightsIndex++];
					auto xL = floatArray[deformWeightsIndex++] * scale;
					auto yL = floatArray[deformWeightsIndex++] * scale;

					if (hasFFD)
					{
						xL += deformVertices[deformVertexIndex++];
						yL += deformVertices[deformVertexIndex++];
					}

					xG += (matrix.a * xL + matrix.c * yL + matrix.tx) * weight;
					yG += (matrix.b * xL + matrix.d * yL + matrix.ty) * weight;
				}
			}

			// Update local verticles positions
			{
				auto& verticesDisplay = _renderDisplay->verticesDisplay;
				const auto& verticesInTriagles = _renderDisplay->verticesInTriagles;

				for (const auto& vert : verticesInTriagles[i])
				{
					verticesDisplay.setPosition(vert, { xG, yG,  (float)_zOrder });
				}
			}
		}
	}

	// Just FFD
	else if (hasFFD)
	{
		const auto data = verticesData->data;
		const int16_t* intArray = data->intArray;
		const float* floatArray = data->floatArray;

		const std::size_t vertexCount = intArray[verticesData->offset + (std::size_t)BinaryOffset::MeshVertexCount];
		const std::size_t vertexOffset = intArray[verticesData->offset + (std::size_t)BinaryOffset::MeshFloatOffset];

		const auto scale = _armature->_armatureData->scale; // @warn what with _textureScale?

		for (std::size_t i = 0, I = vertexCount * 2; i < I; i += 2)
		{
			const auto traingleIndex = i / 2;

			// Calculate new position
			const auto xG = floatArray[vertexOffset + i] * scale + deformVertices[i];
			const auto yG = floatArray[vertexOffset + i + 1] * scale + deformVertices[i + 1];

			// Update local verticles positions
			{
				auto& verticesDisplay = _renderDisplay->verticesDisplay;
				const auto& verticesInTriagles = _renderDisplay->verticesInTriagles;

				for (const auto& vert : verticesInTriagles[traingleIndex])
				{
					verticesDisplay.setPosition(vert, { xG, yG, 0.f });
				}
			}
		}
	}
}

void SF3DSlot::_identityTransform()
{
	//_renderDisplay->setMatrix();
}

void SF3DSlot::_updateTransform()
{
	glm::vec3 pos (
		globalTransformMatrix.tx,
		globalTransformMatrix.ty,
		(float)_zOrder
	);

	if (_renderDisplay.get() == _rawDisplay || _renderDisplay.get() == _meshDisplay)
	{
		pos.x -= (globalTransformMatrix.a * _pivotX + globalTransformMatrix.c * _pivotY);
		pos.y -= (globalTransformMatrix.b * _pivotX + globalTransformMatrix.d * _pivotY);
	}
	else if (!_childArmature)
	{
		pos.x -= (globalTransformMatrix.a + globalTransformMatrix.c);
		pos.y -= (globalTransformMatrix.b + globalTransformMatrix.d);
	}

	_renderDisplay->setMatrix(globalTransformMatrix, pos, _textureScale);
}

void SF3DSlot::_onClear()
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
