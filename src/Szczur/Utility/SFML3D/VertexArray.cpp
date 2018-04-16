#include "VertexArray.hpp"

#include <glad/glad.h>

namespace sf3d {
	VertexArray::VertexArray(size_t size, unsigned int storageUsage) :
		_size(size),
		_storageUsage(storageUsage) {
		glGenBuffers(1, &_VBO);
		glGenVertexArrays(1, &_VAO);

		Vertex defaultVert;

		glBindVertexArray(_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, _VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*size, nullptr, storageUsage);

		for (size_t i = 0; i < size; ++i)
			glBufferSubData(GL_ARRAY_BUFFER, i * sizeof(Vertex), sizeof(Vertex), &defaultVert);


		// position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
							  sizeof(Vertex),
							  (void*)(0)
		);
		// color
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
							  sizeof(Vertex),
							  (void*)(sizeof(Vertex::position))
		);
		// texCoord
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

	void VertexArray::resize(size_t size) {
		glBindVertexArray(_VAO);
		GLuint temp;
		glGenBuffers(1, &temp);
		glBindBuffer(GL_ARRAY_BUFFER, temp);
		glBufferData(GL_ARRAY_BUFFER, _size * sizeof(Vertex), nullptr, _storageUsage);

		glBindBuffer(GL_COPY_READ_BUFFER, _VBO);
		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_ARRAY_BUFFER, 0, 0, _size * sizeof(Vertex));


		glBindBuffer(GL_ARRAY_BUFFER, _VBO);
		glBindBuffer(GL_COPY_READ_BUFFER, temp);
		glBufferData(GL_ARRAY_BUFFER, size * sizeof(Vertex), nullptr, _storageUsage);

		Vertex defaultVert;

		for (size_t i = 0; i < size; ++i)
			glBufferSubData(GL_ARRAY_BUFFER, i * sizeof(Vertex), sizeof(Vertex), &defaultVert);

		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_ARRAY_BUFFER, 0, 0, ((_size>size) ? size : _size) * sizeof(Vertex));

		_size = size;

		glBindBuffer(GL_ARRAY_BUFFER, NULL);
		glBindBuffer(GL_COPY_READ_BUFFER, NULL);

		glBindVertexArray(NULL);
		glDeleteBuffers(1, &temp);
	}

	void VertexArray::setPosition(size_t index, const glm::vec3 & position) {
		if(index < _size) {
			_startEdit(index)->position = position;
			_endEdit();
		}
	}

	void VertexArray::setColor(size_t index, const glm::vec4 & color) {
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
