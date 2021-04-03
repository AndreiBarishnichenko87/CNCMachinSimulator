#include "cameraCAD.h"
#include <iostream>

namespace camera {

	CameraCAD::CameraCAD(const glm::vec3 &rotateAxis, float rotateAngle, float distance)
		: m_Quat(glm::angleAxis(rotateAngle, rotateAxis)), m_DistanceToCenter(distance) { }
	
	void CameraCAD::turn(float xoffset, float yoffset) {
		glm::quat qUpRot = glm::angleAxis(glm::radians(-xoffset), upDir());
		glm::quat qRightRot = glm::angleAxis(glm::radians(yoffset), rightDir());
		m_Quat = qRightRot * qUpRot * m_Quat;
	}
	
	glm::vec3 CameraCAD::cameraPos() const {
		return frontDir() * (-m_DistanceToCenter);
	}
	glm::vec3 CameraCAD::frontDir() const {
		return glm::normalize(glm::mat3_cast(m_Quat) * glm::vec3(0.0f, 0.0f, -1.0f));
	}
	glm::vec3 CameraCAD::upDir() const {
		return glm::normalize(glm::mat3_cast(m_Quat) * glm::vec3(0.0f, 1.0f, 0.0f));		
	}
	glm::vec3 CameraCAD::rightDir() const {
		return glm::normalize(glm::mat3_cast(m_Quat) * glm::vec3(1.0f, 0.0f, 0.0f));		
	}
	
	glm::mat4 CameraCAD::getLookAtMatrix() const {
		return glm::lookAt(cameraPos(), glm::vec3(), upDir());
	}
	
}