#include "rotateShape3D.h"

namespace graphics {
	
	RotateShape3D::RotateShape3D(const glm::vec3 &rotateAxis, float rotateAngle) 
		: m_Quat(glm::angleAxis(rotateAngle, rotateAxis)) { }
	
	RotateShape3D::RotateShape3D(const glm::mat4 &matrix4)
		: m_Quat(glm::quat_cast(matrix4)) { }
	
	void RotateShape3D::rotate(float xoffset, float yoffset, glm::vec3 frontViewDir, glm::vec3 rightViewDir, glm::vec3 upViewDir) {
		glm::vec3 subRotDirection = rightViewDir * xoffset + upViewDir * yoffset;
		glm::vec3 rotateDir = glm::cross(glm::normalize(subRotDirection), frontViewDir);
		glm::quat incrementRotQuat = glm::angleAxis(glm::radians(glm::length(subRotDirection)), glm::normalize(rotateDir));
		m_Quat = incrementRotQuat * glm::normalize(m_Quat);
	}
	
	glm::mat4 RotateShape3D::getMat4() const {
		return glm::mat4_cast(m_Quat);
	}
}