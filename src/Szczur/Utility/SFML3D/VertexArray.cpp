#include "VertexArray.hpp"

#include <glad/glad.h>

namespace sf3d {
	VertexArray::VertexArray(size_t size, unsigned int storageUsage) :
		_size(size),
		_storageUsage(storageUsage) {
		glGenBuffers(1, &_VBO);
		glGenVertexArrays(1, &_VAO);

		Vertex defaultVert{{0.f, 0.f, 0.f}, {1.f, 1.f, 1.f}, {0.f, 0.f}};


		glBindVertexArray(_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, _VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*size, nullptr, storageUsage);

		glBufferSubData(GL_ARRAY_BUFFER, 0 * sizeof(Vertex), sizeof(Vertex), &defaultVert);

		glBufferSubData(GL_ARRAY_BUFFER, 1 * sizeof(Vertex), sizeof(Vertex), &defaultVert);

		glBufferSubData(GL_ARRAY_BUFFER, 2 * sizeof(Vertex), sizeof(Vertex), &defaultVert);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
							  sizeof(Vertex),
							  (void*)(0)
		);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
							  sizeof(Vertex),
							  (void*)(sizeof(Vertex::position))
		);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
							  sizeof(Vertex),
							  (void*)(sizeof(Vertex::position) + sizeof(Vertex::color))
		);

		//  **** **** **** | **** **** **** | **** ****

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);


		glBindBuffer(GL_ARRAY_BUFFER, NULL);
		glBindVertexArray(NULL);
	}

	VertexArray::~VertexArray() {
		glDeleteBuffers(1, &_VBO);
		glDeleteVertexArrays(1, &_VAO);
	}

	void VertexArray::setPosition(size_t index, const glm::vec3 & position) {
		if(index < _size) {
			_startEdit(index)->position = position;
			_endEdit();
		}
	}

	void VertexArray::setColor(size_t index, const glm::vec3 & color) {
		if(index < _size) {
			_startEdit(index)->color = color;
			_endEdit();
		}
	}

	void VertexArray::setTexCoord(size_t index, const glm::vec2 & texCoord) {
		if(index < _size) {
			_startEdit(index)->texCoord = texCoord;
			_endEdit();
		}
	}

	void VertexArray::set(size_t index, const Vertex & vertex) {
		if(index < _size) {
			*_startEdit(index) = vertex;
			_endEdit();
		}
	}

	unsigned int VertexArray::getPrimitiveType() const {
		return _primitveType;
	}

	unsigned int VertexArray::getStorageUsage() const {
		return _storageUsage;
	}

	void VertexArray::setPrimitveType(unsigned int type) {
		_primitveType = type;
	}

	size_t VertexArray::getSize() const {
		return _size;
	}

	void VertexArray::draw(RenderTarget& target, RenderStates states) const {
		//target.draw(this, states);

	}

	void VertexArray::draw() {
		glBindVertexArray(_VAO);

		glDrawArrays(GL_TRIANGLES, 0, _size);

		glBindVertexArray(NULL);
	}

	void VertexArray::bind() const {
		glBindVertexArray(_VAO);
	}

	Vertex * VertexArray::_startEdit(size_t index) {
		glBindBuffer(GL_ARRAY_BUFFER, _VBO);
		return ((Vertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY)) + index;
	}

	void VertexArray::_endEdit() {
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, NULL);
	}

}