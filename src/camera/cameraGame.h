#ifndef CAMERA_GAME_H
#define CAMERA_GAME_H

#include <glm/glm.hpp>
#include <exception>

namespace camera {
	
	const float DEFAULT_MOVE_SENS = 5.0f;
	const float DEFAULT_TURN_SENS = 0.3f;
	const float LIMIT_PITCH = 89.0f;
	const float MY_PI = acos(-1.0);
	
	inline float radianToDegree(float radian) { return (180.0f * radian) / MY_PI; }
	
	class CameraGame {
	public:
		class BadCameraException : std::logic_error {
		public:
			BadCameraException(const std::string &msg) : logic_error("BadCameraException::" + msg) {}
		};
	public:
		enum UpAxis{X, Y, Z};
		enum MoveDirection {FORWARD, BACK, LEFT, RIGHT};
	private:
		glm::vec3 m_Pos;
		glm::vec3 m_WorldUp;
		glm::vec3 m_Front;
		glm::vec3 m_Right;
		glm::vec3 m_Up;
		glm::vec3 (CameraGame::*calculateFront)(float, float) const;
		
		float m_Yaw, m_Pitch;
		float m_MoveSpeed = DEFAULT_MOVE_SENS;
		float m_MouseSens = DEFAULT_TURN_SENS;
	private:
		void updateCameraVectors();
	private:
		float calculateYawWorldY();
		float calculateYawWorldZ();
		float calculateYawWorldX();
		float calculatePitchWorldY();
		float calculatePitchWorldZ();
		float calculatePitchWorldX();
		glm::vec3 calcFrontWorldY(float yaw, float pitch) const;
		glm::vec3 calcFrontWorldZ(float yaw, float pitch) const;
		glm::vec3 calcFrontWorldX(float yaw, float pitch) const;
	public:
		CameraGame() = delete;
		CameraGame(const CameraGame&) = delete;
		CameraGame& operator=(const CameraGame&) = delete;
		explicit CameraGame(glm::vec3 cameraPos, UpAxis upAxis, glm::vec3 cameraTarget = glm::vec3(0.0f));
	public:
		void move(MoveDirection moveType, float offset);
		void turn(float xOffset, float yOffset);

		glm::vec3 position() const { return m_Pos; }
		glm::vec3 frontDir() const { return m_Front; }
		glm::vec3 upDir() const { return m_Up; }
		glm::vec3 rightDir() const { return m_Right; }

		inline void setMoveSpeed(float sensitivity) { m_MoveSpeed = sensitivity; }
		inline void setMouseSpeed(float sensitivity) { m_MouseSens = sensitivity; }
		inline float getMoveSpeed() { return m_MoveSpeed; }
		inline float getMouseSpeed() { return m_MouseSens; }
		
		inline void setNewPos(glm::vec3 newPositionCamera) { m_Pos = newPositionCamera; }
		
		glm::mat4 getLookAtMatrix() const;
		glm::mat4 getTransformMatrix() const;
		~CameraGame() {}
	};

}

#endif //CAMERA_GAME_H