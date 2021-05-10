#ifndef BASE_CAMERA_H
#define BASE_CAMERA_H

#include <stdexcept>
#include <glm/glm.hpp>
#include <string>

namespace camera {

	class BaseCamera {
	public:
		class BadCameraException : public std::logic_error {
		public:
			BadCameraException(const std::string &msg) : std::logic_error("BadCameraException::" + msg) {}
		};
	public:
		enum CameraType : int8_t {GAME = 1, CAD };
	private:
		CameraType m_TypeCamera;
	protected:
		explicit BaseCamera(CameraType cameraType) : m_TypeCamera(cameraType) { }
	public:
		virtual ~BaseCamera() { }
	public:
		virtual glm::vec3 cameraPos() const = 0;
		virtual glm::vec3 frontDir() const = 0;
		virtual glm::vec3 upDir() const = 0;
		virtual glm::vec3 rightDir() const = 0;
		virtual glm::mat4 getLookAtMatrix() const = 0;
	public:
		inline CameraType getCameraType() const { return m_TypeCamera; }
	};
}

#endif //BASE_CAMERA_H