#ifndef _STL_PARSER_H
#define _STL_PARSER_H

#define GLM_FORCE_RADIANS

#include <string>
#include <vector>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

namespace graphics {

	struct dataStructSTL {
		glm::vec3 vertexPos;
		glm::vec3 normalVec;
	};
	
	class ParserSTL {
	private:
		std::vector<dataStructSTL> modelMesh;
	private:
		void readSTL(const std::string &fileName);
	public:
		using typeElementSTL = dataStructSTL;
		ParserSTL(const std::string& fileName);
		
		size_t sizeElement() const { return sizeof(typeElementSTL); }
		size_t countElement() const { return modelMesh.size(); }
		size_t sizeMashBuffer() const { return modelMesh.size() * sizeof(typeElementSTL); }
		
		bool mashIsEmpty() const { return modelMesh.empty(); }
		
		const void* getVoidPtr() { return static_cast<void*>(modelMesh.data()); }
		void readFile(const std::string &fileName);
	};
}

#endif // !_STL_PARSER_H
