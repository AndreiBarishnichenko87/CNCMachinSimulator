#ifndef _STL_PARSER_H
#define _STL_PARSER_H

#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <string>
#include <cstdint> 
#include <exception>

#include "../graphics/dataStructure.h"

namespace parserSTL {

	class ParserSTL {
	private:
		enum FieldInByte {HEADER = 80, NUMBER_OF_TRIANGELS = 4, NORMAL_VECTOR = 12, VERTEX_COORD_3 = 36, SKEEP = 2};
	private:
		std::fstream mBindFile;
		graphics::TriangleData mTriangle;
		std::uint32_t mCountTriangles;
		std::uint32_t mRestTriangles;
	public:
		class BadParserSTL : public std::logic_error {
		public:
			BadParserSTL(const std::string &msg) : logic_error("BadParserSTL::" + msg) {}
		};
	public:
		ParserSTL() : mCountTriangles(0), mRestTriangles(0) { }
		explicit ParserSTL(const std::string& fileName);
		
		std::uint32_t countTriangles() const { return mCountTriangles; }
		std::uint32_t restTriangles() const { return mRestTriangles; }
		
		const graphics::TriangleData& nextTriangle();
		const graphics::TriangleData& curTriangle();
		
		explicit operator bool() const noexcept { return static_cast<bool>(mBindFile); }
	};
}

#endif // !_STL_PARSER_H
