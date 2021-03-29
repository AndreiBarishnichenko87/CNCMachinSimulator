#include <iostream>
#include <complex>

#include "camera.h"
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>


namespace camera {

	CameraGame::CameraGame(glm::vec3 cameraPos, glm::vec3 cameraTarget)
		: m_Pos(cameraPos)
	{
		if (cameraPos.x == cameraTarget.x && cameraPos.y == cameraTarget.y && cameraPos.z == cameraTarget.z)
			throw BadCameraException("constructor::target_is_equvalent_cameraPos");
			
		//m_WorldUp = glm::vec3(1.0f, 0.0f, 0.0f);
		//m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		m_WorldUp = glm::vec3(0.0f, 0.0f, 1.0f);
		
		m_Front = glm::normalize(cameraTarget - cameraPos);

		//m_Yaw = calculateYawWorldX();
		//m_Pitch = calculatePitchWorldX();
		
		//m_Yaw = calculateYawWorldY(m_Front);
		//m_Pitch = calculatePitchWorldY(m_Front);
		
		m_Yaw = calculateYawWorldZ();
		m_Pitch = calculatePitchWorldZ();
	
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
	
	void CameraGame::updateCameraVectors()
    {
		//m_Front = calcFrontWorldX();
		//m_Front = calcFrontWorldY();
		m_Front = calcFrontWorldZ();
        m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
        m_Up    = glm::normalize(glm::cross(m_Right, m_Front));
    }
	
	glm::vec3 CameraGame::calcFrontWorldX() const {
		glm::vec3 front = glm::vec3(0.0f);
		front.z = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		front.y = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));			
		front.x = sin(glm::radians(m_Pitch));
		return glm::normalize(front);
	}
	glm::vec3 CameraGame::calcFrontWorldY() const {
		glm::vec3 front = glm::vec3(0.0f);
		front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		front.y = sin(glm::radians(m_Pitch));
		front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));			
		return glm::normalize(front);
	}
	glm::vec3 CameraGame::calcFrontWorldZ() const {
		glm::vec3 front = glm::vec3(0.0f);
		front.y = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		front.x = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));			
		front.z = sin(glm::radians(m_Pitch));
		return glm::normalize(front);
	}

	glm::mat4 CameraGame::getLookAtMatrix() const { 
		return glm::lookAt(m_Pos, m_Pos + m_Front, m_Up);
	}
	
}