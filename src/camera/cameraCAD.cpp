#include "cameraCAD.h"

namespace camera {

	CameraCAD::CameraCAD(const glm::vec3 &rotateAxis, float rotateAngle, float distance, float sensitivity)
		: BaseCamera(BaseCamera::CameraType::CAD),  m_Quat(glm::angleAxis(rotateAngle, rotateAxis)), m_DistanceToCenter(distance), m_Sensitivity(sensitivity) { }
	
	void CameraCAD::turn(float xoffset, float yoffset) {
		glm::quat qUpRot = glm::angleAxis(glm::radians(-xoffset) * m_Sensitivity, upDir());
		glm::quat qRightRot = glm::angleAxis(glm::radians(yoffset) * m_Sensitivity, rightDir());
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
	
	glm::mat4 CameraCAD::getTransformMatrix() const {
		glm::mat4 matCam = glm::mat4_cast(m_Quat);
		matCam = glm::translate(matCam, glm::vec3(0.0f, 0.0f, -m_DistanceToCenter));
		return matCam;
	}
	
}