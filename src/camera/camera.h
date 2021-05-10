#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

namespace camera {
	
	enum WorldUpVector : uint8_t { X, Y, Z };
	glm::vec3 getConstFrontVec(WorldUpVector vectorType);
	glm::vec3 getConstRightVec(WorldUpVector vectorType);
	glm::vec3 getConstUpVec(WorldUpVector vectorType);
	
	class CameraGeometryData {
	public:
		CameraGeometryData(glm::vec3 pos, WorldUpVector worldUp = WorldUpVector::Z);
	public:
		void setUpDir(glm::vec3 vec) { m_Up = vec; }
		void setRightDir(glm::vec3 vec) { m_Right = vec; }
		void setFrontDir(glm::vec3 vec) { m_Front = vec; }
		void setPos(glm::vec3 vec) { m_Pos = vec; }
	public:
		const glm::vec3& getUpDir() { return m_Up; }
		const glm::vec3& getRightDir() { return m_Right; }
		const glm::vec3& getFrontDir() { return m_Front; }
		const glm::vec3& getPos() { return m_Pos; }
	public:
		WorldUpVector getUpWorldVec_t() { return m_UpWorldVec; }
		glm::vec3 getUpWorldVec() { return getConstUpVec(m_UpWorldVec); }
	private:
		WorldUpVector m_UpWorldVec;
		glm::vec3 m_Pos;
		glm::vec3 m_Right, m_Front, m_Up;
	};

}

#endif //CAMERA_H