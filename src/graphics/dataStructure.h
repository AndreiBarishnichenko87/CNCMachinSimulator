#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#include <glm/glm.hpp>

namespace graphics {
	
	typedef glm::vec3 VertexPros;
	typedef glm::vec3 VertexNormal;
	typedef glm::vec3 VertexColor;
	
	struct TriangleData {
		VertexColor m_Normal = glm::vec3(0.0f);
		VertexPros m_VertexCoord[3] = {VertexPros(0.0f)};
	};
	
}

#endif //DATA_STRUCTURE_H