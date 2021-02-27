#ifndef SHADER_H_
#define SHADER_H_

#include <glad/glad.h>
#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include <string>

namespace graphics {
	
	class Shader{
	private:
		GLuint m_ID;
	public:
		Shader(const std::string &vertexShader, const std::string &fragmentShader);
		~Shader();
		void use() const;
		void del() const;
		void setMat4f(const std::string &name, glm::mat4 value) const;
		void setVec3f(const std::string &name, glm::vec3 value) const;
		void setFloat(const std::string &name, GLfloat value) const;
		void setInt(const std::string &name, GLint value) const;
	};
	
}

#endif // SHADER_H_