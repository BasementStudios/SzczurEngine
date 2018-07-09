#include "VertexArray.hpp"

#include <stdexcept> // out_of_range
#include <glad/glad.h>

#include "Vertex.hpp"

namespace sf3d {
	VertexArray::VertexArray(size_t size, unsigned int storageUsage) :
	_vertices(size),
	_storageUsage(storageUsage) {
		glGenBuffers(1, &_VBO);
		glGenVertexArrays(1, &_VAO);


		glBindVertexArray(_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, _VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*_vertices.size(), &(_vertices[0]), _storageUsage);

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



		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
	}


	VertexArray::~VertexArray() {
		if(_VBO)
			glDeleteBuffers(1, &_VBO);
		if(_VAO)
			glDeleteVertexArrays(1, &_VAO);
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
		return _vertices.size();
	}

	Vertex & VertexArray::add() {
		_toResize = true;
		_toUpdate = true;
		return _vertices.emplace_back();
	}

	Vertex & VertexArray::add(const Vertex & vertex) {
		_toResize = true;
		_toUpdate = true;
		return _vertices.emplace_back(vertex);
	}

	void VertexArray::resize(size_t newSize) {
		_toResize = true;
		_toUpdate = true;
		_vertices.resize(newSize);
	}

	void VertexArray::draw(RenderTarget& /*target*/, RenderStates /*states*/) const {
		//target.draw(this, states);
	}

	void VertexArray::bind() const {
		glBindVertexArray(_VAO);
	}

	void VertexArray::update() const {
		if(_toUpdate && _VBO && _VAO) {
			_update();
			_toResize = false;
			_toUpdate = false;
			_min = static_cast<size_t>(-1);
			_max = 0u;
		}
	}

	void VertexArray::_update() const {
		glBindBuffer(GL_ARRAY_BUFFER, _VBO);
		if(_toResize)
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*_vertices.size(), &(_vertices[0]), _storageUsage);
		else
			glBufferSubData(GL_ARRAY_BUFFER, _min*sizeof(Vertex), (_max - _min + 1)*sizeof(Vertex), &(_vertices[_min]));
		//
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	Vertex& VertexArray::operator[](size_t index) {
		if(index < _vertices.size()) {
			if(index < _min)
				_min = index;
			if(index > _max)
				_max = index;
			_toUpdate = true;
			return _vertices[index];
		}
		throw std::out_of_range{"Vertex array out of bound"};
	}

}
