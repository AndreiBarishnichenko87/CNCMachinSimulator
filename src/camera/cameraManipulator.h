#ifndef CAMERA_MANIPULATOR_H
#define CAMERA_MANIPULATOR_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "../application/eventSystem/event.h"
#include "camera.h"

namespace camera {
	
	static const float PI = acos(-1.0);
	static inline float radToDeg(float radian) { return (180.0f * radian) / PI; }
	
	// Abstract move class manipulator
	// -------------------------------
	class CameraMoveManipulator {
	public:
		virtual void moveFront(CameraGeometryData *camera, float offset) = 0;
		virtual void moveBack(CameraGeometryData *camera, float offset) = 0;
		virtual void moveRight(CameraGeometryData *camera, float offset) = 0;
		virtual void moveLeft(CameraGeometryData *camera, float offset) = 0;
		virtual void moveUp(CameraGeometryData *camera, float offset) { }
		virtual void moveDown(CameraGeometryData *camera, float offset) { }
	public:
		virtual ~CameraMoveManipulator() { }
	};
	
	// Concret move class manipulator
	// ------------------------------
	class MoveAlongCameraAxis : public CameraMoveManipulator {
	public:
		void moveFront(CameraGeometryData *camera, float offset) { camera->setPos(camera->getPos() + camera->getFrontDir() * offset); }
		void moveBack(CameraGeometryData *camera, float offset) { camera->setPos(camera->getPos() - camera->getFrontDir() * offset); }
		void moveRight(CameraGeometryData *camera, float offset) { camera->setPos(camera->getPos() - camera->getRightDir() * offset); }
		void moveLeft(CameraGeometryData *camera, float offset) { camera->setPos(camera->getPos() + camera->getRightDir() * offset); }
	};

	class MoveInCameraUpWorld : public CameraMoveManipulator {
	public:
		void moveFront(CameraGeometryData *camera, float offset) { camera->setPos(camera->getPos() + calcFrontVec(camera) * offset); }
		void moveBack(CameraGeometryData *camera, float offset) { camera->setPos(camera->getPos() - calcFrontVec(camera) * offset); }
		void moveRight(CameraGeometryData *camera, float offset) { camera->setPos(camera->getPos() - calcRightVec(camera) * offset); }
		void moveLeft(CameraGeometryData *camera, float offset) { camera->setPos(camera->getPos() + calcRightVec(camera) * offset); }
	private:
		glm::vec3 calcFrontVec(CameraGeometryData *camera) { return glm::normalize(glm::cross(calcRightVec(camera), camera->getUpWorldVec())); }
		glm::vec3 calcRightVec(CameraGeometryData *camera) { return glm::normalize(glm::cross(camera->getUpWorldVec(), camera->getFrontDir())); }
	};
	
	// Abstract Rotate Camera Manipulator
	// ----------------------------------
	class CameraRotateManipulator {
	public:
		virtual void rotateRight(CameraGeometryData *camera, float offset) = 0;
		virtual void rotateLeft(CameraGeometryData *camera, float offset) = 0;
		virtual void rotateUp(CameraGeometryData *camera, float offset) = 0;
		virtual void rotateDown(CameraGeometryData *camera, float offset) = 0;
		virtual void rotateClockWise(CameraGeometryData *camera, float offset) { }
		virtual void rotatecounterClockWise(CameraGeometryData *camera, float offset) { }
	public:
		virtual ~CameraRotateManipulator() { }
	};
	
	// Concret Rotate Camera Manipulator
	// ---------------------------------
	
	class CameraRotateGameWay : public CameraRotateManipulator {
	public:
		CameraRotateGameWay(float limitUpAngel);
	public:
		void rotateRight(CameraGeometryData *camera, float offset) override;
		void rotateLeft(CameraGeometryData *camera, float offset) override;
		void rotateUp(CameraGeometryData *camera, float offset) override;
		void rotateDown(CameraGeometryData *camera, float offset) override;
	private:
		void updateFuncPtr(CameraGeometryData *camera);
		float calcPitchWorldX(CameraGeometryData *camera);
		float calcPitchWorldY(CameraGeometryData *camera);
		float calcPitchWorldZ(CameraGeometryData *camera);
		float calcYawWorldX(CameraGeometryData *camera);
		float calcYawWorldY(CameraGeometryData *camera);
		float calcYawWorldZ(CameraGeometryData *camera);
		glm::vec3 calcFrontWorldY(float yaw, float pitch) const;
		glm::vec3 calcFrontWorldZ(float yaw, float pitch) const;
		glm::vec3 calcFrontWorldX(float yaw, float pitch) const;
		void updateCameraVectors(CameraGeometryData *camera, float yaw, float pitch);
	private:
		float (CameraRotateGameWay::*m_FunPtrCalcYaw)(CameraGeometryData*);
		float (CameraRotateGameWay::*m_FunPtrCalcPitch)(CameraGeometryData*);
		glm::vec3 (CameraRotateGameWay::*m_FunPtrCalcFront)(float, float) const;
	private:
		float m_LimitAngle;
		WorldUpVector m_WorldUp;
	};
	
	class CameraRotateCADWay : public CameraRotateManipulator {
	public:
		CameraRotateCADWay(WorldUpVector vecType) : m_Quat(glm::angleAxis(0.0f, getConstUpVec(vecType))) { }
	public:
		void rotateRight(CameraGeometryData *camera, float offset) override;
		void rotateLeft(CameraGeometryData *camera, float offset) override;
		void rotateUp(CameraGeometryData *camera, float offset) override;
		void rotateDown(CameraGeometryData *camera, float offset) override;
		void rotateClockWise(CameraGeometryData *camera, float offset) override;
		void rotatecounterClockWise(CameraGeometryData *camera, float offset) override;
	private:
		void updateCameraVectors(CameraGeometryData *camera);
	private:
		glm::quat m_Quat;
	};
	
	class RotateDecoratorCAD : public CameraRotateManipulator {
	public:
		RotateDecoratorCAD(CameraRotateManipulator &decoretedObj, glm::vec3 centerOfRotation = glm::vec3(0.0f)) : 
			m_DecoratedObj(decoretedObj), m_CenterOfRot(centerOfRotation) { }
	public:
		void rotateRight(CameraGeometryData *camera, float offset) override;
		void rotateLeft(CameraGeometryData *camera, float offset) override;
		void rotateUp(CameraGeometryData *camera, float offset) override;
		void rotateDown(CameraGeometryData *camera, float offset) override;
	private:
		void calculateStepDir(CameraGeometryData *camera);
		void updateCameraPos(CameraGeometryData *camera);
	private:
		CameraRotateManipulator &m_DecoratedObj;
		glm::vec3 m_CenterOfRot;
		float m_StepFront = 0.0f;
		float m_StepRight = 0.0f;
		float m_StepUp = 0.0f;
	};
	
}

#endif //CAMERA_MANIPULATOR_H