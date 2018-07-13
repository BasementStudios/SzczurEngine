#pragma once

#include <ostream>

#include <glm/vec3.hpp>

inline std::ostream& operator<< (std::ostream& stream, glm::vec3 const& value)
{
	stream << "{x: " << value.x << ", y:" << value.y << ", z:" << value.z << "}";
	return stream;
}
