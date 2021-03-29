#ifndef SHAPEFACTORY_H
#define SHAPEFACTORY_H

#include <memory>
#include <string>

#include "baseShape.h"

namespace graphics {
	
	class ShapeFactory {
	private:
		ShapeFactory(const ShapeFactory&) = delete;
		ShapeFactory() {}
		ShapeFactory& operator=(const ShapeFactory&) = delete;
	public:
		static ShapeFactory& instance();
	public:
		std::shared_ptr<BaseShape> createShapeSTL(const std::string &fileName);
	};
	
}


#endif //SHAPEFACTORY_H