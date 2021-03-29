#ifndef BASESHAPE_H
#define BASESHAPE_H

namespace graphics {

	enum class AttribPosition : int {COORD = 0, NORMAL, COLOR};
	
	class BaseShape {
	public:
		virtual void draw() = 0;
		virtual ~BaseShape() {}
	};

}


#endif //BASESHAPE_H

