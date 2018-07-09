#pragma once

#include <vector>

#include "Drawable.hpp"
#include "PrimitiveType.hpp"
#include "Vertex.hpp"

namespace sf3d
{

class VertexArray
{
public:

    using Vertices_t = std::vector<Vertex>;

    ///
    VertexArray() = delete;

    ///
    VertexArray(PrimitiveType type = TriangleFan);

    ///
    VertexArray(PrimitiveType type, size_t size);

    ///
    VertexArray(PrimitiveType type, size_t size, const Vertex& value);

    ///
    VertexArray(PrimitiveType type, const Vertex* vertices, size_t size);

    ///
    VertexArray(PrimitiveType type, const Vertex* begin, const Vertex* end);

    ///
    VertexArray(const VertexArray& rhs);

    ///
    VertexArray& operator = (const VertexArray& rhs);

    ///
    VertexArray(VertexArray&& rhs) noexcept;

    ///
    VertexArray& operator = (VertexArray&& rhs) noexcept;

    ///
    ~VertexArray();

    ///
    void clear();

    ///
    VertexArray& assign(PrimitiveType type, size_t size);

    ///
    VertexArray& assign(PrimitiveType type, size_t size, const Vertex& value);

    ///
    VertexArray& assign(PrimitiveType type, const Vertex* vertices, size_t size);

    ///
    VertexArray& assign(PrimitiveType type, const Vertex* begin, const Vertex* end);

    ///
    void resize(size_t size);

    ///
    void resize(size_t size, const Vertex& value);

    ///
    void setVertex(size_t index, const Vertex& vertex);

    ///
    Vertex& getVertex(size_t index);

    ///
    const Vertex& getVertex(size_t index) const;

    ///
    Vertex& operator [] (size_t index);

    ///
    const Vertex& operator [] (size_t index) const;

    ///
    void setPrimitiveType(PrimitiveType type);

    ///
    PrimitiveType getPrimitiveType() const;

    ///
    Vertex* getData();

    ///
    const Vertex* getData() const;

    ///
    size_t getSize() const;

    ///
    size_t getBytesCount() const;

    ///
    bool isEmpty() const;

    ///
    bool isValid() const;

    /// Returns { left, top, width, height } 2D bounds of vertices
    glm::vec4 getBounds() const;

    ///
    void bind() const;

    ///
    void unbind() const;

	///
	void update() const;

private:

    ///
    void _init();

    ///
    void _destroy();

    Vertices_t _vertices;
    PrimitiveType _type;
    GLuint _vao;
    GLuint _vbo;
    mutable size_t _lowerIndex;
    mutable size_t _upperIndex;
    mutable bool _needsUpdate;
    mutable bool _needsReallocate;

};

}
