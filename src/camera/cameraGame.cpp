#include <iostream>
#include <complex>

#include "cameraGame.h"
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

namespace camera {

	CameraGame::CameraGame(glm::vec3 cameraPos, UpAxis upAxis, glm::vec3 cameraTarget)
		: m_Pos(cameraPos)
	{
		if (cameraPos.x == cameraTarget.x && cameraPos.y == cameraTarget.y && cameraPos.z == cameraTarget.z)
			throw BadCameraException("constructor::target_is_equvalent_cameraPos");
		
		m_Front = glm::normalize(cameraTarget - cameraPos);
		
		if (upAxis == UpAxis::Y) {
			m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
			m_Yaw = calculateYawWorldY();
			m_Pitch = calculatePitchWorldY();
			calculateFront = &CameraGame::calcFrontWorldY;
		} else if (upAxis == UpAxis::X) {
			m_WorldUp = glm::vec3(1.0f, 0.0f, 0.0f);
			m_Yaw = calculateYawWorldX();
			m_Pitch = calculatePitchWorldX();
			calculateFront = &CameraGame::calcFrontWorldX;
		} else if (upAxis == UpAxis::Z) {
			m_WorldUp = glm::vec3(0.0f, 0.0f, 1.0f);
			m_Yaw = calculateYawWorldZ();
			m_Pitch = calculatePitchWorldZ();
			calculateFront = &CameraGame::calcFrontWorldZ;
		} else {
			throw BadCameraException("type of upAxis is undefine");
		}
		updateCameraVectors();
	}
	
	float CameraGame::calculateYawWorldX() {
		return radianToDegree(arg(std::complex<float>(m_Front.z, m_Front.y)));
	}
	float CameraGame::calculateYawWorldY() {
		return radianToDegree(arg(std::complex<float>(m_Front.x, m_Front.z)));
	}
	float CameraGame::calculateYawWorldZ() {
		return radianToDegree(arg(std::complex<float>(m_Front.y, m_Front.x)));
	}
	
	float CameraGame::calculatePitchWorldX() {
		float pitch = (1.0f - glm::dot(m_Front, glm::vec3(0.0f, m_Front.y,m_Front.z))) * 90.0f;
		if (m_Front.x < 0) pitch *= -1;
		return pitch;
	}
	float CameraGame::calculatePitchWorldY() {
		float pitch = (1.0f - glm::dot(m_Front, glm::vec3(m_Front.x, 0.0f, m_Front.z))) * 90.0f;
		if (m_Front.y < 0) pitch *= -1;
		return pitch;
	}
	float CameraGame::calculatePitchWorldZ() {
		float pitch = (1.0f - glm::dot(m_Front, glm::vec3(m_Front.x, m_Front.y, 0.0f))) * 90.0f;
		if (m_Front.z < 0) pitch *= -1;
		return pitch;
	}
	
	void CameraGame::move(MoveDirection moveType, float offset) {
		switch(static_cast<int>(moveType)) {
			case MoveDirection::FORWARD: 
				m_Pos += m_Front * (offset * m_MoveSpeed); break;
			case MoveDirection::BACK: 
				m_Pos -= m_Front * (offset * m_MoveSpeed); break;
			case MoveDirection::LEFT: 
				m_Pos -= m_Right * (offset * m_MoveSpeed); break;
			case MoveDirection::RIGHT: 
				m_Pos += m_Right * (offset * m_MoveSpeed); break;
			default: break;
		}
	}
	
	void CameraGame::turn(float xOffset, float yOffset) {
		m_Yaw += (xOffset * m_MouseSens);
		m_Pitch += (yOffset * m_MouseSens);
		if(m_Pitch > LIMIT_PITCH)
		  m_Pitch =  LIMIT_PITCH;
		if(m_Pitch < -LIMIT_PITCH)
		  m_Pitch = -LIMIT_PITCH;
		updateCameraVectors();
	}

	glm::vec3 CameraGame::calcFrontWorldX(float yaw, float pitch) const {
		glm::vec3 front = glm::vec3(0.0f);
		front.z = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(yaw)) * cos(glm::radians(pitch));			
		front.x = sin(glm::radians(pitch));
		return glm::normalize(front);
	}
	glm::vec3 CameraGame::calcFrontWorldY(float yaw, float pitch) const {
		glm::vec3 front = glm::vec3(0.0f);
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));			
		return glm::normalize(front);
	}
	glm::vec3 CameraGame::calcFrontWorldZ(float yaw, float pitch) const {
		glm::vec3 front = glm::vec3(0.0f);
		front.y = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.x = sin(glm::radians(yaw)) * cos(glm::radians(pitch));			
		front.z = sin(glm::radians(pitch));
		return glm::normalize(front);
	}

	void CameraGame::updateCameraVectors()
    {
		m_Front = (this->*calculateFront)(m_Yaw, m_Pitch);
        m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
        m_Up    = glm::normalize(glm::cross(m_Right, m_Front));
    }
	
	glm::mat4 CameraGame::getLookAtMatrix() const { 
		return glm::lookAt(m_Pos, m_Pos + m_Front, m_Up);
	}
	
	glm::mat4 CameraGame::getTransformMatrix() const {
		
		glm::mat4 matrix = glm::translate(glm::mat4(1.0f), m_Pos);
		
		glm::vec4 UpVec4 = glm::vec4(m_Up.x, m_Up.y, m_Up.z, 0.0f);
		glm::vec4 RightVec4 = glm::vec4(m_Right.x, m_Right.y, m_Right.z, 0.0f);
		glm::vec4 FrontVec4 = glm::vec4(m_Front.x, m_Front.y, m_Front.z, 0.0f);
		glm::vec4 WcomponentVec4 = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	
		matrix *= glm::mat4(UpVec4, RightVec4, FrontVec4, WcomponentVec4);
		return matrix;
	}
	
}