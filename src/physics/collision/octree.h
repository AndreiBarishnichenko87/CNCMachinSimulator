#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <utility>
#include <cstdint> 

namespace octree {

	struct Node {
		glm::vec3 m_CentrPoint;
		float m_Width, m_Heigth;
		Node* m_childNode[8];
		std::vector<uint32_t, uint8_t> m_PointsContainer;
	};

	class Octree {
		unsigned int mSize
	};

}