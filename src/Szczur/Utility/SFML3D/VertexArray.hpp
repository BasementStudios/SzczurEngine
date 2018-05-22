#pragma once

#include "Vertex.hpp"
#include "Drawable.hpp"
namespace sf3d {
	class VertexInterface {
	public:
		VertexInterface(GLuint VBO, size_t index);

		void setPosition(const glm::vec3& position);
		void move(const glm::vec3& offset); 
 
		void setColor(const glm::vec4& color); 

		void setTexCoord(const glm::vec2& pos); 
	private:
		GLuint _VBO;
		size_t _index;
	};

	class VertexArray : public Drawable {
	public:
		VertexArray() = delete;
		VertexArray(size_t size, unsigned int storageUsage = GL_STREAM_DRAW);
		~VertexArray();

		void setPosition(size_t index, const glm::vec3& position);
		void setColor(size_t index, const glm::vec4& color);
		void setTexCoord(size_t index, const glm::vec2& texCoord);
		void set(size_t index, const Vertex& vertex);

		unsigned int getPrimitiveType() const;
		unsigned int getStorageUsage() const;

		void setPrimitveType(unsigned int type);

		size_t getSize() const;

		void resize(size_t size);

		void bind() const;

		virtual void draw(RenderTarget& target, RenderStates states) const override;
		void draw();

		VertexInterface operator[](size_t index) const;
	private:
		unsigned int _VAO;
		unsigned int _VBO;

		unsigned int _primitveType{GL_TRIANGLE_FAN};
		unsigned int _storageUsage;

		size_t _size;

		Vertex* _startEdit(size_t index);

		void _endEdit();
	};
}
