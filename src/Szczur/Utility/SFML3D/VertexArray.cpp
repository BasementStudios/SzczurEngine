#include "VertexArray.hpp"

#include <vector>
#include <algorithm>
#include <stdexcept> // out_of_range

#include <glad/glad.h>

#include "RenderTarget.hpp"
#include "RenderStates.hpp"
#include "PrimitiveType.hpp"
#include "Vertex.hpp"

template <typename T, typename Class>
constexpr const void* offsetPtrOf(T Class::*member)
{
    return &(reinterpret_cast<Class*>(0)->*member);
}

namespace sf3d
{

inline constexpr size_t minIndex = 0;
inline constexpr size_t maxIndex = -1;

VertexArray::VertexArray(PrimitiveType type)
    : _vertices {}
    , _type { type }
    , _vao { 0 }
    , _vbo { 0 }
    , _lowerIndex { maxIndex }
    , _upperIndex { minIndex }
    , _needsUpdate { false }
    , _needsReallocate { false }
{
    _init();
}

VertexArray::VertexArray(size_t size, PrimitiveType type)
    : _vertices { size, Vertex{} }
    , _type { type }
    , _vao { 0 }
    , _vbo { 0 }
    , _lowerIndex { maxIndex }
    , _upperIndex { minIndex }
    , _needsUpdate { false }
    , _needsReallocate { false }
{
    _init();
}

VertexArray::VertexArray(size_t size, const Vertex& value, PrimitiveType type)
    : _vertices { size, value }
    , _type { type }
    , _vao { 0 }
    , _vbo { 0 }
    , _lowerIndex { maxIndex }
    , _upperIndex { minIndex }
    , _needsUpdate { false }
    , _needsReallocate { false }
{
    _init();
}

VertexArray::VertexArray(const Vertex* vertices, size_t size, PrimitiveType type)
    : _vertices { vertices, vertices + size }
    , _type { type }
    , _vao { 0 }
    , _vbo { 0 }
    , _lowerIndex { maxIndex }
    , _upperIndex { minIndex }
    , _needsUpdate { false }
    , _needsReallocate { false }
{
    _init();
}

VertexArray::VertexArray(const Vertex* begin, const Vertex* end, PrimitiveType type)
    : _vertices { begin, end }
    , _type { type }
    , _vao { 0 }
    , _vbo { 0 }
    , _lowerIndex { maxIndex }
    , _upperIndex { minIndex }
    , _needsUpdate { false }
    , _needsReallocate { false }
{
    _init();
}

VertexArray::VertexArray(const VertexArray& rhs)
    : VertexArray { rhs._vertices.data(), rhs._vertices.size(), rhs._type }
{

}

VertexArray& VertexArray::operator = (const VertexArray& rhs)
{
    if (this != &rhs)
    {
        _destroy();

        _vertices = rhs._vertices;
        _type = rhs._type;

        _init();
    }

    return *this;
}

VertexArray::VertexArray(VertexArray&& rhs) noexcept
    : _vertices { std::move(rhs._vertices) }
    , _type { rhs._type }
    , _vao { rhs._vao }
    , _vbo { rhs._vbo }
    , _lowerIndex { rhs._lowerIndex }
    , _upperIndex { rhs._upperIndex }
    , _needsUpdate { rhs._needsUpdate }
    , _needsReallocate { rhs._needsReallocate }
{
    rhs._vao = 0;
    rhs._vbo = 0;
    rhs._lowerIndex = maxIndex;
    rhs._upperIndex = minIndex;
    rhs._needsUpdate = false;
    rhs._needsReallocate = false;
}

VertexArray& VertexArray::operator = (VertexArray&& rhs) noexcept
{
    if (this != &rhs)
    {
        _destroy();

        _vertices = std::move(rhs._vertices);
        _type = rhs._type;
        _vao = rhs._vao;
        _vbo = rhs._vbo;
        _lowerIndex = rhs._lowerIndex;
        _upperIndex = rhs._upperIndex;
        _needsUpdate = rhs._needsUpdate;
        _needsReallocate = rhs._needsReallocate;

        rhs._vao = 0;
        rhs._vbo = 0;
        rhs._lowerIndex = maxIndex;
        rhs._upperIndex = minIndex;
        rhs._needsReallocate = false;
        rhs._needsUpdate = false;
    }

    return *this;
}

VertexArray::~VertexArray()
{
    _destroy();
}

void VertexArray::clear()
{
    _vertices.clear();
}

VertexArray& VertexArray::assign(size_t size, PrimitiveType type)
{
    if (size != 0)
    {
        _needsUpdate = true;
        _needsReallocate = _vertices.capacity() < size;

        _lowerIndex = minIndex;
        _upperIndex = size - 1;

        _vertices.assign(size, Vertex{});
        _type = type;
    }

    return *this;
}

VertexArray& VertexArray::assign(size_t size, const Vertex& value, PrimitiveType type)
{
    if (size != 0)
    {
        _needsUpdate = true;
        _needsReallocate = _vertices.capacity() < size;

        _lowerIndex = minIndex;
        _upperIndex = size - 1;

        _vertices.assign(size, value);
        _type = type;
    }

    return *this;
}

VertexArray& VertexArray::assign(const Vertex* vertices, size_t size, PrimitiveType type)
{
    if (size != 0)
    {
        _needsUpdate = true;
        _needsReallocate = _vertices.capacity() < size;

        _lowerIndex = minIndex;
        _upperIndex = size - 1;

        _vertices.assign(vertices, vertices + size);
        _type = type;
    }

    return *this;
}

VertexArray& VertexArray::assign(const Vertex* begin, const Vertex* end, PrimitiveType type)
{
    const ptrdiff_t distance = end - begin;

    if (distance > 0)
    {
        _needsUpdate = true;
        _needsReallocate = _vertices.capacity() < static_cast<size_t>(distance);

        _lowerIndex = minIndex;
        _upperIndex = distance - 1;

        _vertices.assign(begin, end);
        _type = type;
    }

    return *this;
}

void VertexArray::resize(size_t size)
{
    if (_vertices.size() != size)
    {
        _needsUpdate = true;
        _needsReallocate = _vertices.capacity() < size;

        _lowerIndex = minIndex;
        _upperIndex = size - 1;

        _vertices.resize(size);
    }
}

void VertexArray::resize(size_t size, const Vertex& value)
{
    if (_vertices.size() != size)
    {
        _needsUpdate = true;
        _needsReallocate = _vertices.capacity() < size;

        _lowerIndex = minIndex;
        _upperIndex = size - 1;

        _vertices.resize(size, value);
    }
}

void VertexArray::setVertex(size_t index, const Vertex& vertex)
{
    _vertices[index] = vertex;

    _lowerIndex = std::min(_lowerIndex, index);
    _upperIndex = std::max(_upperIndex, index);

    _needsUpdate = true;
}

Vertex& VertexArray::getVertex(size_t index)
{
    _lowerIndex = std::min(_lowerIndex, index);
    _upperIndex = std::max(_upperIndex, index);

    _needsUpdate = true;

    return _vertices[index];
}

const Vertex& VertexArray::getVertex(size_t index) const
{
    return _vertices[index];
}

Vertex& VertexArray::operator [] (size_t index)
{
    return getVertex(index);
}

const Vertex& VertexArray::operator [] (size_t index) const
{
    return getVertex(index);
}

void VertexArray::setPrimitiveType(PrimitiveType type)
{
    _type = type;
}

PrimitiveType VertexArray::getPrimitiveType() const
{
    return _type;
}

Vertex* VertexArray::getData()
{
    _lowerIndex = minIndex;
    _upperIndex = _vertices.size() - 1;

    return _vertices.empty() ? nullptr : _vertices.data();
}

const Vertex* VertexArray::getData() const
{
    return _vertices.empty() ? nullptr : _vertices.data();
}

size_t VertexArray::getSize() const
{
    return _vertices.size();
}

size_t VertexArray::getBytesCount() const
{
    return _vertices.size() * sizeof(Vertex);
}

bool VertexArray::isEmpty() const
{
    return _vertices.empty();
}

bool VertexArray::isValid() const
{
    return _vao != 0 && _vbo != 0;
}

glm::vec4 VertexArray::getBounds() const
{
    if (!_vertices.empty())
    {
        auto left   = _vertices[0].position.x;
        auto top    = _vertices[0].position.y;
        auto right  = _vertices[0].position.x;
        auto bottom = _vertices[0].position.y;

        for (size_t v = 1; v < _vertices.size(); ++v)
        {
            const auto pos = _vertices[v].position;

            if (pos.x < left)
            {
                left = pos.x;
            }
            else if (pos.x > right)
            {
                right = pos.x;
            }

            if (pos.y < top)
            {
                top = pos.y;
            }
            else if (pos.y > bottom)
            {
                bottom = pos.y;
            }
        }

        return { left, top, right - left, bottom - top };
    }
    else
    {
        return { 0.0f, 0.0f, 0.0f, 0.0f };
    }
}

void VertexArray::bind() const
{
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
}

void VertexArray::unbind() const
{
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexArray::update() const
{
    if (_needsUpdate && _vao != 0 && _vbo != 0)
    {
        bind();

        if (_needsReallocate)
        {
            glBufferData(GL_ARRAY_BUFFER, getBytesCount(), getData(), GL_DYNAMIC_DRAW);

            _needsReallocate = false;
        }
        else
        {
            const GLintptr offset = sizeof(Vertex) * _lowerIndex;
            const GLsizeiptr size = sizeof(Vertex) * (_upperIndex - _lowerIndex + 1);

            glBufferSubData(GL_ARRAY_BUFFER, offset, size, getData() + _lowerIndex);
        }

        _lowerIndex = maxIndex;
        _upperIndex = minIndex;

        _needsUpdate = false;

        unbind();
    }
}

void VertexArray::draw(RenderTarget& target, RenderStates states) const
{
    target.draw(*this, states);
}

void VertexArray::_init()
{
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);

    bind();

    glBufferData(GL_ARRAY_BUFFER, getBytesCount(), std::as_const(*this).getData(), GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, decltype(Vertex::position)::length(), GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetPtrOf(&Vertex::position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, decltype(Vertex::color)::length(), GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetPtrOf(&Vertex::color));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, decltype(Vertex::texCoord)::length(), GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetPtrOf(&Vertex::texCoord));

    unbind();
}

void VertexArray::_destroy()
{
    clear();

    glDeleteVertexArrays(1, &_vao);
    _vao = 0;

    glDeleteBuffers(1, &_vbo);
    _vbo = 0;

    _lowerIndex = maxIndex;
    _upperIndex = minIndex;

    _needsUpdate = false;
    _needsReallocate = false;
}

}
