#pragma once

#include <vector>

namespace sf3d {
    class RenderTarget;
}
#include "RenderStates.hpp"
#include "Drawable.hpp"
#include "PrimitiveType.hpp"
#include "Vertex.hpp"

namespace sf3d
{

class VertexArray : public Drawable
{
public:

    using Vertices_t = std::vector<Vertex>;

    ///
    VertexArray() = delete;

    ///
    VertexArray(PrimitiveType type = TriangleFan);

    ///
    VertexArray(size_t size, PrimitiveType type = TriangleFan);

    ///
    VertexArray(size_t size, const Vertex& value, PrimitiveType type = TriangleFan);

    ///
    VertexArray(const Vertex* vertices, size_t size, PrimitiveType type = TriangleFan);

    ///
    VertexArray(const Vertex* begin, const Vertex* end, PrimitiveType type = TriangleFan);

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
    VertexArray& assign(size_t size, PrimitiveType type = TriangleFan);

    ///
    VertexArray& assign(size_t size, const Vertex& value, PrimitiveType type = TriangleFan);

    ///
    VertexArray& assign(const Vertex* vertices, size_t size, PrimitiveType type = TriangleFan);

    ///
    VertexArray& assign(const Vertex* begin, const Vertex* end, PrimitiveType type = TriangleFan);

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

    ///
    virtual void draw(RenderTarget& target, RenderStates states = RenderStates::Default) const override;

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
