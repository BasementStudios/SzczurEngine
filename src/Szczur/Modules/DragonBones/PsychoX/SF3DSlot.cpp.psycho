
/** @file SF3DSlot.hpp
 ** @author Piotr Krupa (piotrkrupa06@gmail.com)
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi@gmail.com>
 **/

#include "SF3DSlot.hpp"

#include <SFML/Graphics/Color.hpp>

#include <glad/glad.h>

#include "Szczur/Utility/SFML3D/Vertex.hpp"
#include "Szczur/Utility/SFML3D/VertexArray.hpp"

#include "SF3DTextureData.hpp"

DRAGONBONES_NAMESPACE_BEGIN

void SF3DSlot::_updateVisible()
{
	_renderDisplay->visible = _parent->getVisible();
}

void SF3DSlot::_updateBlendMode()
{
	// if (_renderDisplay)
	// {
	// 	switch (_blendMode)
	// 	{
	// 		case BlendMode::Normal:
	// 			_renderDisplay->blendMode = sf::BlendMode();
	// 			break;
	// 		case BlendMode::Add:
	// 			_renderDisplay->blendMode = sf::BlendAdd;
	// 			break;
	// 		case BlendMode::Multiply:
	// 			_renderDisplay->blendMode = sf::BlendMultiply;
	// 			break;
	// 		default:
	// 			_renderDisplay->blendMode = sf::BlendMode();
	// 			break;
	// 	}
	// }
	// else if (_childArmature)
	// {
	// 	for (const auto slot : _childArmature->getSlots())
	// 	{
	// 		slot->_blendMode = _blendMode;
	// 		slot->_updateBlendMode();
	// 	}
	// }
}

void SF3DSlot::_updateColor()
{
	if (_renderDisplay)
	{
		_renderDisplay->setColor(sf::Color(
			static_cast<uint8_t>(_colorTransform.alphaMultiplier * 255.f),
			static_cast<uint8_t>(_colorTransform.redMultiplier   * 255.f),
			static_cast<uint8_t>(_colorTransform.greenMultiplier * 255.f),
			static_cast<uint8_t>(_colorTransform.blueMultiplier  * 255.f)
		));
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
	const auto currentTextureData = static_cast<SF3DTextureData*>(_textureData);

	if (_displayIndex >= 0 && _display != nullptr && currentTextureData != nullptr) {
		if (currentTextureData->texture != nullptr) {
			// Mesh
			if (currentVerticesData != nullptr) {
				const auto data = currentVerticesData->data;
				const int16_t* intArray = data->intArray;
				const float* floatArray = data->floatArray;
				
				const std::size_t vertexCount = intArray[currentVerticesData->offset + (std::size_t)BinaryOffset::MeshVertexCount];
				const std::size_t triangleCount = intArray[currentVerticesData->offset + (std::size_t)BinaryOffset::MeshTriangleCount];
				
				// Apply texture scaling
				_textureScale = 1.f; // @warn no scale on mesh?

				// Prepare renderDisplay fields
				_renderDisplay->texture = currentTextureData->texture;
				_renderDisplay->verticesDisplay.resize(triangleCount * 3);
				_renderDisplay->verticesInTriagles.resize(vertexCount);
				_renderDisplay->primitiveType = GL_TRIANGLES; // @todo . can be optimized using TriangleStrip

				// Load (common) verticles
				std::vector<sf3d::Vertex> vertices(vertexCount);
				{
					const auto& region = currentTextureData->region;

					const std::size_t vertexOffset = intArray[currentVerticesData->offset + (std::size_t)BinaryOffset::MeshFloatOffset];
					const std::size_t uvOffset = vertexOffset + vertexCount * 2;

					for (std::size_t i = 0, I = vertexCount * 2; i < I; i += 2) {
						const auto vertexIndex = i / 2;
						
						const auto x = floatArray[vertexOffset + i];
						const auto y = floatArray[vertexOffset + i + 1];
						
						auto u = floatArray[uvOffset + i];
						auto v = floatArray[uvOffset + i + 1];
						if (currentTextureData->rotated) {
							v = (1.0f - v);
						}
						
						const float tX = region.x + u * region.width;
						const float tY = region.y + v * region.height;

						vertices[vertexIndex] = {{x, y, 0.f}, {1.f, 1.f, 1.f, 1.f}, {tX, tY}};
					}
				}

				// Setup verticles to display
				{
					std::vector<std::size_t> vertexIndices(triangleCount * 3);

					for (std::size_t i = 0, I = triangleCount * 3; i < I; ++i) {
						vertexIndices.emplace_back(intArray[currentVerticesData->offset + (std::size_t)BinaryOffset::MeshVertexIndices + i]);
					}

					_renderDisplay->verticesInTriagles.clear();

					for (std::size_t i = 0, I = triangleCount * 3; i < I; i++) {
						_renderDisplay->verticesInTriagles[vertexIndices[i]].emplace_back(i);
						_renderDisplay->verticesDisplay.set(i, vertices[vertexIndices[i]]);
					}
				}

				const auto isSkinned = currentVerticesData->weight != nullptr;
				if (isSkinned) {
					_identityTransform();
				}
			} 
			
			// Normal texture
			else { 
				// Apply texture scaling
				_textureScale = currentTextureData->parent->scale * _armature->_armatureData->scale; // @warn no scale on mesh?

				// Prepare renderDisplay fields
				_renderDisplay->texture = currentTextureData->texture;
				_renderDisplay->verticesDisplay.resize(4);
				_renderDisplay->verticesInTriagles.resize(0);
				_renderDisplay->primitiveType = GL_TRIANGLE_STRIP;
				
				// Setup verticles
				{
					const auto& region = currentTextureData->region;
					
					const glm::vec2& size = currentTextureData->getSize();
					float tsX = size.x * region.width;
					float tsY = size.y * region.height;
					
					_renderDisplay->verticesDisplay.set(0u, {{0.f, 0.f, 0.f}, {1.f, 1.f, 1.f, 1.f}, {region.x,     region.y}});
					_renderDisplay->verticesDisplay.set(1u, {{0.f, tsY, 0.f}, {1.f, 1.f, 1.f, 1.f}, {region.x,     region.height}});
					_renderDisplay->verticesDisplay.set(2u, {{tsX, 0.f, 0.f}, {1.f, 1.f, 1.f, 1.f}, {region.width, region.y}});
					_renderDisplay->verticesDisplay.set(3u, {{tsX, tsY, 0.f}, {1.f, 1.f, 1.f, 1.f}, {region.width, region.height}});
				}
			}

			// Request futher updates
			_visibleDirty = true;
			_blendModeDirty = true;
			_colorDirty = true;

			return;
		}
	}

	// Try prevent instant crashing
	_renderDisplay->visible = false;
}

void SF3DSlot::_updateMesh()
{
	const auto& deformVertices = _deformVertices->vertices;
	const auto& deformBones    = _deformVertices->bones;
	const auto& verticesData   = _deformVertices->verticesData;
	const auto& weightData = verticesData->weight;

	const auto hasFFD = !deformVertices.empty();

	// Weightness
	if (weightData != nullptr) {
		const auto data = verticesData->data;
		const int16_t* intArray = data->intArray;
		const float* floatArray = data->floatArray;
		
		const std::size_t vertexCount = intArray[verticesData->offset + (std::size_t)BinaryOffset::MeshVertexCount];
		const std::size_t weightFloatOffset = intArray[weightData->offset + (std::size_t)BinaryOffset::WeigthFloatOffset];
		
		const auto scale = _armature->_armatureData->scale; // @warn what with _textureScale?

		std::size_t deformWeightsIndex = (std::size_t)weightFloatOffset;
		std::size_t deformVertexIndex = 0;
		std::size_t deformBonesIndex = weightData->offset + (std::size_t)BinaryOffset::WeigthBoneIndices + deformBones.size();

		for (std::size_t i = 0; i < vertexCount; ++i) {
			float xG = 0.0f;
			float yG = 0.0f;
			
			// Calculate new position taking into account bone weights
			const std::size_t boneCount = intArray[deformBonesIndex++];
			for (std::size_t j = 0; j < boneCount; ++j) {
				const std::size_t boneIndex = intArray[deformBonesIndex++];
				const auto bone = deformBones[boneIndex];
				
				if (bone != nullptr) {
					const auto& matrix = bone->globalTransformMatrix;
					
					const auto weight = floatArray[deformWeightsIndex++];
					auto xL = floatArray[deformWeightsIndex++] * scale;
					auto yL = floatArray[deformWeightsIndex++] * scale;

					if (hasFFD) {
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
				
				for (const auto& vert : verticesInTriagles[i]) {
					verticesDisplay.setPosition(vert, {xG, yG, 0.f});
				}
			}
		}
	}

	// Just FFD
	else if (hasFFD) {
		const auto data = verticesData->data;
		const int16_t* intArray = data->intArray;
		const float* floatArray = data->floatArray;
		
		const std::size_t vertexCount = intArray[verticesData->offset + (std::size_t)BinaryOffset::MeshVertexCount];
		const std::size_t vertexOffset = intArray[verticesData->offset + (std::size_t)BinaryOffset::MeshFloatOffset];

		const auto scale = _armature->_armatureData->scale; // @warn what with _textureScale?

		for (std::size_t i = 0, I = vertexCount * 2; i < I; i += 2) {
			const auto traingleIndex = i / 2;

			// Calculate new position
			const auto xG = floatArray[vertexOffset + i] * scale + deformVertices[i];
			const auto yG = floatArray[vertexOffset + i + 1] * scale + deformVertices[i + 1];

			// Update local verticles positions
			{
				auto& verticesDisplay = _renderDisplay->verticesDisplay;
				const auto& verticesInTriagles = _renderDisplay->verticesInTriagles;
				
				for (const auto& vert : verticesInTriagles[traingleIndex]) {
					verticesDisplay.setPosition(vert, {xG, yG, 0.f});
				}
			}
		}
	}
}

void SF3DSlot::_identityTransform()
{
	_renderDisplay->setMatrix(Matrix(), glm::vec3(), _textureScale);
}

void SF3DSlot::_updateTransform()
{
	glm::vec3 position(
		globalTransformMatrix.tx,
		globalTransformMatrix.ty,
		0.f
	);

	if (_renderDisplay.get() == _rawDisplay || _renderDisplay.get() == _meshDisplay)
	{
		position.x -= (globalTransformMatrix.a * _pivotX + globalTransformMatrix.c * _pivotY);
		position.y -= (globalTransformMatrix.b * _pivotX + globalTransformMatrix.d * _pivotY);
	}
	else if (!_childArmature)
	{
		position.x -= (globalTransformMatrix.a - globalTransformMatrix.c);
		position.y -= (globalTransformMatrix.b - globalTransformMatrix.d);
	}

	_renderDisplay->setMatrix(globalTransformMatrix, position, _textureScale);
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
