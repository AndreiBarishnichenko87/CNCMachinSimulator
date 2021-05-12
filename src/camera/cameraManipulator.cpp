#include <complex>
#include <cmath>
#include <iostream>


#include "cameraManipulator.h"

#define PRINTVEC3(vec3) #vec3 << ": <" << vec3.x << "> <" << vec3.y << "> <" << vec3.z << "> || "

namespace camera {
	
	// CAMERA MOVE AROUND CENTER
	// -------------------------
	void RotateDecoratorCAD::calculateStepDir(CameraGeometryData *camera) { 
		glm::vec3 vecFromCamPosToCentrRot = m_CenterOfRot - camera->getPos();
		float distFromCamToCenterRot = glm::length(vecFromCamPosToCentrRot);

		if(distFromCamToCenterRot != 0)	{ vecFromCamPosToCentrRot = glm::normalize(vecFromCamPosToCentrRot); }

		m_StepFront = distFromCamToCenterRot * glm::dot(camera->getFrontDir(), vecFromCamPosToCentrRot);
		m_StepRight = distFromCamToCenterRot * glm::dot(camera->getRightDir(), vecFromCamPosToCentrRot);
		m_StepUp = distFromCamToCenterRot * glm::dot(camera->getUpDir(), vecFromCamPosToCentrRot);
	}
	
	void RotateDecoratorCAD::updateCameraPos(CameraGeometryData *camera) {
		glm::vec3 camPos = m_CenterOfRot;
		camPos -= camera->getFrontDir() * m_StepFront;
		camPos -= camera->getRightDir() * m_StepRight;
		camPos -= camera->getUpDir() * m_StepUp;
		camera->setPos(camPos);
	}
	
	void RotateDecoratorCAD::rotateRight(CameraGeometryData *camera, float offset) {
		calculateStepDir(camera);
		m_DecoratedObj.rotateRight(camera, offset);
		updateCameraPos(camera);
	}
	
	void RotateDecoratorCAD::rotateLeft(CameraGeometryData *camera, float offset) {
		calculateStepDir(camera);
		m_DecoratedObj.rotateLeft(camera, offset);
		updateCameraPos(camera);
	}
	
	void RotateDecoratorCAD::rotateUp(CameraGeometryData *camera, float offset) {
		calculateStepDir(camera);
		m_DecoratedObj.rotateUp(camera, offset);
		updateCameraPos(camera);
	}
	
	void RotateDecoratorCAD::rotateDown(CameraGeometryData *camera, float offset) {
		calculateStepDir(camera);
		m_DecoratedObj.rotateDown(camera, offset);
		updateCameraPos(camera);
	}
	// CAMERA ROTATE GAME WAY
	// ----------------------
	CameraRotateGameWay::CameraRotateGameWay(float limitUpAngel) {
		m_LimitAngle = ((limitUpAngel > 89.0f) || (limitUpAngel < 0.0f)) ? 89.0f : limitUpAngel;
		m_WorldUp = WorldUpVector::Z;
		m_FunPtrCalcPitch = CameraRotateGameWay::calcPitchWorldZ;
		m_FunPtrCalcYaw = CameraRotateGameWay::calcYawWorldZ;
		m_FunPtrCalcFront = CameraRotateGameWay::calcFrontWorldZ;
	}
	
	void CameraRotateGameWay::updateFuncPtr(CameraGeometryData *camera) {
		if(m_WorldUp != camera->getUpWorldVec_t()) {
			m_WorldUp = camera->getUpWorldVec_t();
			switch(m_WorldUp) {
				case WorldUpVector::X:
					m_FunPtrCalcPitch = CameraRotateGameWay::calcPitchWorldX;
					m_FunPtrCalcYaw = CameraRotateGameWay::calcYawWorldX;
					m_FunPtrCalcFront = CameraRotateGameWay::calcFrontWorldX;
					break;
				case WorldUpVector::Y:
					m_FunPtrCalcPitch = CameraRotateGameWay::calcPitchWorldY;
					m_FunPtrCalcYaw = CameraRotateGameWay::calcYawWorldY;
					m_FunPtrCalcFront = CameraRotateGameWay::calcFrontWorldY;
					break;
				case WorldUpVector::Z:
					m_FunPtrCalcPitch = CameraRotateGameWay::calcPitchWorldZ;
					m_FunPtrCalcYaw = CameraRotateGameWay::calcYawWorldZ;
					m_FunPtrCalcFront = CameraRotateGameWay::calcFrontWorldZ;
					break;
			}
		}
	}

	float CameraRotateGameWay::calcPitchWorldX(CameraGeometryData *camera) {
		return radToDeg(asin(camera->getFrontDir().x));
	}
	float CameraRotateGameWay::calcPitchWorldY(CameraGeometryData *camera) {
		return radToDeg(asin(camera->getFrontDir().y));
	}
	float CameraRotateGameWay::calcPitchWorldZ(CameraGeometryData *camera) {
		return radToDeg(asin(camera->getFrontDir().z));
	}

	float CameraRotateGameWay::calcYawWorldX(CameraGeometryData *camera) {
		return radToDeg(arg(std::complex<float>(camera->getFrontDir().z, camera->getFrontDir().y)));
	}
	float CameraRotateGameWay::calcYawWorldY(CameraGeometryData *camera) {
		return radToDeg(arg(std::complex<float>(camera->getFrontDir().x, camera->getFrontDir().z)));
	}
	float CameraRotateGameWay::calcYawWorldZ(CameraGeometryData *camera) {
		return radToDeg(arg(std::complex<float>(camera->getFrontDir().y, camera->getFrontDir().x)));
	}

	glm::vec3 CameraRotateGameWay::calcFrontWorldX(float yaw, float pitch) const {
		glm::vec3 front;
		front.x = sin(glm::radians(pitch));
		front.y = sin(glm::radians(yaw)) * cos(glm::radians(pitch));			
		front.z = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		return glm::normalize(front);
	}
	glm::vec3 CameraRotateGameWay::calcFrontWorldY(float yaw, float pitch) const {
		glm::vec3 front;
		front.y = sin(glm::radians(pitch));
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		return glm::normalize(front);
	}
	glm::vec3 CameraRotateGameWay::calcFrontWorldZ(float yaw, float pitch) const {
		glm::vec3 front;
		front.z = sin(glm::radians(pitch));
		front.x = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		return glm::normalize(front);
	}

	void CameraRotateGameWay::rotateRight(CameraGeometryData *camera, float offset) {
		updateFuncPtr(camera);
		updateCameraVectors(camera, (this->*m_FunPtrCalcYaw)(camera) + offset, (this->*m_FunPtrCalcPitch)(camera));
	}
	
	void CameraRotateGameWay::rotateLeft(CameraGeometryData *camera, float offset) {
		updateFuncPtr(camera);
		updateCameraVectors(camera, (this->*m_FunPtrCalcYaw)(camera) - offset, (this->*m_FunPtrCalcPitch)(camera));
	}
	
	void CameraRotateGameWay::rotateUp(CameraGeometryData *camera, float offset) {
		updateFuncPtr(camera);
		updateCameraVectors(camera, (this->*m_FunPtrCalcYaw)(camera), (this->*m_FunPtrCalcPitch)(camera) + offset);
	}
	
	void CameraRotateGameWay::rotateDown(CameraGeometryData *camera, float offset) {
		updateFuncPtr(camera);
		updateCameraVectors(camera, (this->*m_FunPtrCalcYaw)(camera), (this->*m_FunPtrCalcPitch)(camera) - offset);
	}

	void CameraRotateGameWay::updateCameraVectors(CameraGeometryData *camera, float yaw, float pitch) {
		if(pitch > m_LimitAngle) 
			pitch =  m_LimitAngle;
		else if(pitch < -m_LimitAngle) 
			pitch = -m_LimitAngle;
		camera->setFrontDir((this->*m_FunPtrCalcFront)(yaw, pitch));
        camera->setRightDir(glm::normalize(glm::cross(camera->getUpWorldVec(), camera->getFrontDir())));
        camera->setUpDir(glm::normalize(glm::cross(camera->getFrontDir(), camera->getRightDir())));
    }
	
	// CAMERA ROTATE CAD WAY
	// ---------------------
	void CameraRotateCADWay::rotateRight(CameraGeometryData *camera, float offset) {
		m_Quat = glm::angleAxis(glm::radians(-offset), camera->getUpDir()) * m_Quat;
		updateCameraVectors(camera);
	}
	void CameraRotateCADWay::rotateLeft(CameraGeometryData *camera, float offset) {
		m_Quat = glm::angleAxis(glm::radians(offset), camera->getUpDir()) * m_Quat;
		updateCameraVectors(camera);
	}
	void CameraRotateCADWay::rotateUp(CameraGeometryData *camera, float offset) {
		m_Quat = glm::angleAxis(glm::radians(-offset), camera->getRightDir()) * m_Quat;
		updateCameraVectors(camera);
	}
	void CameraRotateCADWay::rotateDown(CameraGeometryData *camera, float offset) {
		m_Quat = glm::angleAxis(glm::radians(offset), camera->getRightDir()) * m_Quat;
		updateCameraVectors(camera);
	}
	void CameraRotateCADWay::rotateClockWise(CameraGeometryData *camera, float offset) {
		m_Quat = glm::angleAxis(glm::radians(offset), camera->getFrontDir()) * m_Quat;
		updateCameraVectors(camera);
	}
	void CameraRotateCADWay::rotatecounterClockWise(CameraGeometryData *camera, float offset) {
		m_Quat = glm::angleAxis(glm::radians(-offset), camera->getFrontDir()) * m_Quat;
		updateCameraVectors(camera);
	}

	void CameraRotateCADWay::updateCameraVectors(CameraGeometryData *camera) {
		glm::quat qFront = m_Quat * glm::quat(0.0f, getConstFrontVec(camera->getUpWorldVec_t())) * glm::conjugate(m_Quat);
		glm::quat qRight = m_Quat * glm::quat(0.0f, getConstRightVec(camera->getUpWorldVec_t())) * glm::conjugate(m_Quat);
		glm::quat qUp = m_Quat * glm::quat(0.0f, getConstUpVec(camera->getUpWorldVec_t())) * glm::conjugate(m_Quat);
		
		camera->setFrontDir(glm::normalize(glm::vec3(qFront.x, qFront.y, qFront.z)));
		camera->setRightDir(glm::normalize(glm::vec3(qRight.x, qRight.y, qRight.z)));
		camera->setUpDir(glm::normalize(glm::vec3(qUp.x, qUp.y, qUp.z)));
	}
}
