#ifndef ROTATE_SHAPE_3D_H
#define ROTATE_SHAPE_3D_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace graphics {
	
	class RotateShape3D {
	private:
		glm::quat m_Quat;
	public:
		RotateShape3D() {}
		RotateShape3D(const glm::vec3 &rotateAxis, float rotateAngle);
		RotateShape3D(const glm::mat4 &matrix4);
	public:
		void rotate(float xoffset, float yoffset, glm::vec3 frontViewDir, glm::vec3 rightViewDir, glm::vec3 upViewDir);
		glm::mat4 getMat4() const;
	public:
		~RotateShape3D() {}
	};
	
}

#endif //ROTATE_SHAPE_3D_H