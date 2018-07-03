#include "Linear.hpp"

sf3d::Linear::Linear(const glm::vec3 & position, const glm::vec3 & direction) :
_position(position), _direction(direction) {

}

bool sf3d::Linear::contains(const glm::vec3& pos,const glm::vec3& size) const {
	glm::vec3 deltaPosition = pos - _position;

	glm::vec3 temp = _direction * (deltaPosition.z / _direction.z);
	if(temp.x > deltaPosition.x && temp.x < deltaPosition.x + size.x) {
		if(temp.y < deltaPosition.y && temp.y > deltaPosition.y - size.y)
			return true;
	}
	return false;

}

glm::vec3 sf3d::Linear::getProjectionY(const glm::vec3& pos) {
	glm::vec3 deltaPosition = pos - _position;
	return _direction * (deltaPosition.y / _direction.y);
}

glm::vec3 sf3d::Linear::getProjectionZ(const glm::vec3& pos) {
	glm::vec3 deltaPosition = pos - _position;
	return _direction * (deltaPosition.z / _direction.z);
}
