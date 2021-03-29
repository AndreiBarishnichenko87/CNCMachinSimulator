#include "../dataStructure.h"
#include "shapeSTL.h"
#include "../../parserSTL/parserSTL.h"

namespace graphics {

	ShapeSTL::ShapeSTL(const std::string &nameFile) {	
		parserSTL::ParserSTL parser(nameFile);
		TriangleData data = parser.nextTriangle();
		while (parser.restTriangles()) {
			for(unsigned int i = 0; i < 3; ++i) {
				mDataBufferSTL.push_back(data.m_VertexCoord[i]);
				mDataBufferSTL.push_back(data.m_Normal);
			}
			data = parser.nextTriangle();
		}

		glGenBuffers(1, &mVBO);
		glGenVertexArrays(1, &mVAO);
		glBindVertexArray(mVAO);
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferData(GL_ARRAY_BUFFER,  mDataBufferSTL.size() * sizeof(value_type), static_cast<const void*>(mDataBufferSTL.data()), GL_STATIC_DRAW);
		
		int attribPos = static_cast<int>(AttribPosition::COORD);
		glVertexAttribPointer(attribPos, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(value_type), (void*)0);
		glEnableVertexAttribArray(attribPos);
		attribPos = static_cast<int>(AttribPosition::NORMAL);
		glVertexAttribPointer(attribPos, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(value_type), (void*)(sizeof(value_type)));
		glEnableVertexAttribArray(attribPos);
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	
	void ShapeSTL::draw() {
		glBindVertexArray(mVAO);
		glDrawArrays(GL_TRIANGLES, 0, mDataBufferSTL.size());
		glBindVertexArray(0);
	}

	ShapeSTL::~ShapeSTL() {
		glDeleteVertexArrays(1, &mVAO);
		glDeleteBuffers(1, &mVBO);
	}
	
}