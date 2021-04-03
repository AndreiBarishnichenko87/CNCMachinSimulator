#ifndef CAMERA_CAD_H
#define CAMERA_CAD_H

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace camera {
	
	class CameraCAD {
	private:
		glm::quat m_Quat;
		float m_DistanceToCenter;
	public:
		CameraCAD(const CameraCAD&) = delete;
		CameraCAD& operator=(const CameraCAD&) = delete;
	public:
		CameraCAD() : m_DistanceToCenter(100.0f) {}
		CameraCAD(const glm::vec3 &rotateAxis, float rotateAngle, float distance);
	public:
		void turn(float xoffset, float yoffset);
	public:
		glm::vec3 cameraPos() const;
		glm::vec3 frontDir() const;
		glm::vec3 upDir() const;
		glm::vec3 rightDir() const;
		
		glm::mat4 getLookAtMatrix() const;
	public:
		~CameraCAD() {}
	};
	
}

#endif //CAMERA_CAD_H