#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

namespace camera {
	
	class CameraManager {
	public:
		void keyPush(int key, int scancode, int mods);
		void keyRelese(int key, int scancode, int mods);
		void keyRepeat(int key, int scancode, int mods);
		void mousePush(int button, int action, int mods);
		void mouseRelese(int button, int action, int mods);
		void mouseMove(double xpos, double ypos);
	private:
		
	};
	
}

#endif //CAMERA_MANAGER_H