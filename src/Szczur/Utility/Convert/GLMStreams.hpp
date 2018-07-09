
#include <ostream>
#include <glm/vec3.hpp>

std::ostream& operator << (std::ostream& stream, const glm::vec2& vec) {
	return stream << "{x: " << vec.x << ", y: " << vec.y << "}";
}

std::ostream& operator << (std::ostream& stream, const glm::vec3& vec) {
	return stream << "{x: " << vec.x << ", y: " << vec.y << ", z: " << vec.z << "}";
}
