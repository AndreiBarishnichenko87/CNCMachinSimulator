

#include "parserSTL.h"
#include <fstream>
#include <iomanip>
#include <utility>


namespace parserSTL {

	ParserSTL::ParserSTL(const std::string& fileName) : mCountTriangles(0),  mRestTriangles(0) {
			mBindFile.open(fileName.c_str(), std::ios_base::in | std::ios_base::binary);
			mBindFile.seekg (0, mBindFile.end);
			int sizeFile = mBindFile.tellg();
			mBindFile.seekg (0, mBindFile.beg);
			
			if(sizeFile < FieldInByte::NUMBER_OF_TRIANGELS)
				throw BadParserSTL("header field is less then 84 byte");

			mBindFile.seekg(FieldInByte::HEADER, std::ios_base::beg);
			mBindFile.read((char*)(&mCountTriangles), FieldInByte::NUMBER_OF_TRIANGELS);
			mRestTriangles = mCountTriangles;
	}
	
	const graphics::TriangleData& ParserSTL::nextTriangle() {
		mBindFile.read((char*)&mTriangle, FieldInByte::NORMAL_VECTOR + FieldInByte::VERTEX_COORD_3);
		if(mBindFile.gcount() < (FieldInByte::NORMAL_VECTOR + FieldInByte::VERTEX_COORD_3))
			throw BadParserSTL("count triangles is not match as assigned");
		mBindFile.seekg(2, std::ios_base::cur);
		--mRestTriangles;
		return mTriangle;
	}
	
	const graphics::TriangleData& ParserSTL::curTriangle() {
		if (mRestTriangles == mCountTriangles) {
			nextTriangle();
		}
		return mTriangle; 
	}

}