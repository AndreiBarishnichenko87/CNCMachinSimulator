#include "shapeFactory.h"

#include "shapeSTL.h"

namespace graphics {

	// singlet shape factory initializer
	ShapeFactory& ShapeFactory::instance() {
		static ShapeFactory SF;
		return SF;
	}

	// factory shapes methods
	std::shared_ptr<BaseShape> ShapeFactory::createShapeSTL(const std::string &fileName) {
		return std::shared_ptr<BaseShape>(new ShapeSTL(fileName));;
	}

}