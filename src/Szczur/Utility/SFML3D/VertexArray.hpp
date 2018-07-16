#pragma once

#include "Vertex.hpp"
#include "Drawable.hpp"
namespace sf3d {

	class VertexArray : public Drawable {
	public:
		VertexArray() = delete;
		VertexArray(size_t size, unsigned int storageUsage = GL_STREAM_DRAW);
		~VertexArray();



		unsigned int getPrimitiveType() const;
		unsigned int getStorageUsage() const;

		void setPrimitveType(unsigned int type);

		size_t getSize() const;

		Vertex& add();
		Vertex& add(const Vertex& vertex);

		void resize(size_t newSize);

		void bind() const;

		virtual void draw(RenderTarget& target, RenderStates states) const override;

		void update() const;

		Vertex& operator[](size_t index);

	private:
		void _update() const;

		unsigned int _VAO;
		unsigned int _VBO;

		mutable bool _toUpdate{false};
		mutable bool _toResize{false};

		mutable size_t _min{static_cast<size_t>(-1)};
		mutable size_t _max{0u};

		unsigned int _primitveType{GL_TRIANGLE_FAN};

		unsigned int _storageUsage;
		std::vector<Vertex> _vertices;

	};
}
