#ifndef GRAPHICS_MATH_H
#define GRAPHICS_MATH_H

#include <glm/glm.hpp>

namespace graphics {
	
	bool shortestDistToRay(const glm::vec3 &ray, const glm::vec3 point, glm::vec3 getPoint);
	
}

#endif //GRAPHICS_MATH_H