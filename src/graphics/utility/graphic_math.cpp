#include "graphic_math.h"

namespace graphics {
	
	float shortestDistToRay(const glm::vec3 &ray, const glm::vec3 &pointRayBegin, const glm::vec3 point) {
		float distance = glm::dot(glm::normalize(ray), (point - pointRayBegin));
		return distance;
	}
	
}

