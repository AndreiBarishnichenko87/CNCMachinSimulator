#ifndef SHAPESTL_H
#define SHAPESTL_H

#include <glm/glm.hpp>
#include <glad/glad.h>

#include <string>
#include <vector>

#include "baseShape.h"

namespace graphics {
	
	class ShapeFactory;
	
	class ShapeSTL : public BaseShape {
	private:
		friend ShapeFactory;
		ShapeSTL(const std::string &nameFile);
	private:
		GLuint mVBO, mVAO;
		std::vector<glm::vec3> mDataBufferSTL;
		typedef std::vector<glm::vec3>::value_type value_type; 
	public:
		void draw();
		~ShapeSTL();
	};

}

#endif //SHAPESTL_H