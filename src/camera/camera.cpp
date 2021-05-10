#include "camera.h"

namespace camera {

	glm::vec3 getConstFrontVec(WorldUpVector vectorType) {
		switch(vectorType) {
			case WorldUpVector::X:
				return glm::vec3(0.0f, 1.0f, 0.0f);
			case WorldUpVector::Y:
				return glm::vec3(0.0f, 0.0f, 1.0f);
			case WorldUpVector::Z:
				return glm::vec3(1.0f, 0.0f, 0.0f);
		}
		return glm::vec3(0.0f);
	}
	glm::vec3 getConstRightVec(WorldUpVector vectorType) {
		switch(vectorType) {
			case WorldUpVector::X:
				return glm::vec3(0.0f, 0.0f, 1.0f);
			case WorldUpVector::Y:
				return glm::vec3(1.0f, 0.0f, 0.0f);
			case WorldUpVector::Z:
				return glm::vec3(0.0f, 1.0f, 0.0f);
		}
		return glm::vec3(0.0f);
	}
	glm::vec3 getConstUpVec(WorldUpVector vectorType) {
		switch(vectorType) {
			case WorldUpVector::X:
				return glm::vec3(1.0f, 0.0f, 0.0f);
			case WorldUpVector::Y:
				return glm::vec3(0.0f, 1.0f, 0.0f);
			case WorldUpVector::Z:
				return glm::vec3(0.0f, 0.0f, 1.0f);
		}
		return glm::vec3(0.0f);
	}
	
	CameraGeometryData::CameraGeometryData(glm::vec3 pos, WorldUpVector worldUp)
		: m_UpWorldVec(worldUp), m_Pos(pos)
	{
		m_Up = getConstUpVec(m_UpWorldVec);
		m_Right = getConstRightVec(m_UpWorldVec);
		m_Front = getConstFrontVec(m_UpWorldVec);
	}

}
