#ifndef CAMERA_CAD_H
#define CAMERA_CAD_H

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "baseCamera.h"

namespace camera {
	
	class CameraCAD : public BaseCamera {
	private:
		glm::quat m_Quat;
		float m_DistanceToCenter;
		float m_Sensitivity;
	public:
		CameraCAD(const CameraCAD&) = delete;
		CameraCAD& operator=(const CameraCAD&) = delete;
	public:
		CameraCAD() : BaseCamera(BaseCamera::CameraType::CAD), m_DistanceToCenter(100.0f), m_Sensitivity(1.0f) {}
		CameraCAD(const glm::vec3 &rotateAxis, float rotateAngle, float distance, float sensitivity = 1.0f);
	public:
		void turn(float xoffset, float yoffset);
		
		glm::vec3 cameraPos() const override;
		glm::vec3 frontDir() const override;
		glm::vec3 upDir() const override;
		glm::vec3 rightDir() const override;

		glm::mat4 getLookAtMatrix() const override;
		glm::mat4 getTransformMatrix() const;
	public:
		virtual ~CameraCAD() {}
	};
	
}

#endif //CAMERA_CAD_H