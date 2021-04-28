#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS // use for lib glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <iostream>
#include <string>
#include <memory>
#include <cmath>

#include "graphics/shapes/shapeFactory.h"
#include "graphics/dataStructure.h"
#include "parserXML/smartBuffer.h"
#include "parserXML/lexerXML.h"
#include "parserXML/parserXML.h"
#include "parserXML/exceptionParserXML.h"
#include "graphics/shader/shader.h"
#include "graphics/stb/stb_image.h"
#include "parserSTL/parserSTL.h"
#include "camera/baseCamera.h"
#include "camera/cameraGame.h"
#include "camera/cameraCAD.h"
#include "graphics/shapeManipulators/rotateShape3D.h"
#include "application/window/window.h"

#include "application/eventSystem/eventDespatcher.h"
#include "application/eventSystem/windowEvents/mouseMoveEvent.h"
#include "application/eventSystem/windowEvents/mouseButtonEvent.h"
#include "application/eventSystem/windowEvents/mouseScrollEvent.h"
#include "application/eventSystem/windowEvents/keyboardEvent.h"

#include "graphics/imGui/imgui.h"
#include "graphics/imGui/imgui_impl_glfw.h"
#include "graphics/imGui/imgui_impl_opengl3.h"

using std::cout;
using std::cin;
using std::endl;
using graphics::ShapeFactory;

// MACROS FUNCTIONS
#define PRINT(p) std::cout << #p << " => " << p
#define PAUSE_MSG(msg) std::cout << msg << endl; std::cin.get()
#define PRINTVEC4(vec4) cout << "vec4 => x:" << vec4.x << " y:" << vec4.y << " z:" << vec4.z << " w:" << vec4.w << endl
#define PRINTVEC3(vec3) cout << "vec3 => x:" << vec3.x << " y:" << vec3.y << " z:" << vec3.z << endl

void printAllDoc(parserXML::ElementXML element, std::ostream &fout, unsigned int depthTree) {
	
	for(unsigned int i = 0; i < depthTree; ++i) fout << " ";
	fout << "<" << element.elementName() << ">";
	if(element.valueExist())
		fout << " [" << element.elementValue() << "]" << endl;
	else
		fout << endl;
	if(element.countAttribute() != 0) {
		for(unsigned int j = 0; j < element.countAttribute(); ++j) {
			for(unsigned int i = 0; i < depthTree + 1; ++i) fout << " ";
			fout << element.attribNameNum(j) << " = [" << element.attribValueNum(j) << "]" << endl;
		}
	}
	
	++depthTree;
	if(element.countChildElements() != 0) {
		for(unsigned int i = 0; i < element.countChildElements(); ++i) {
			printAllDoc(element.childElementNum(i), fout, depthTree);
		}
	}
}
void test_parserXML() {
	std::ofstream fout("e:\\project\\MyProject\\resourses\\parserXML\\testParser.xml");
	parserXML::ParserXML parser("e:\\project\\MyProject\\resourses\\parserXML\\langs.model.xml");
	parser.parse();
	printAllDoc(parser.getRootElement(), fout, 0);
}

glm::vec3 pointOnSphear(float radius, float angle_XY, float angle_VecZ) {
	glm::vec3 spherPos;
	spherPos.x = radius * sin(glm::radians(angle_XY)) * sin(glm::radians(angle_VecZ));
	spherPos.y = radius * cos(glm::radians(angle_VecZ));
	spherPos.z = radius * cos(glm::radians(angle_XY)) * sin(glm::radians(angle_VecZ));
	return spherPos;
}

//////////////////////////////////////////////
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void processInput(GLFWwindow *window);

bool pickWhenReleaseButton(unsigned int isPress) {
	static bool buttonIsPress = false;
	if (isPress) {
		buttonIsPress = true;
	} else {
		if (buttonIsPress) {
			buttonIsPress = false;
			return true;
		}
	}
	return false;
}

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;

//////////////////////////////////////////////
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

// LINE PRINMITIVE
struct Line {
	glm::vec3 beginLine;
	glm::vec3 endLine;
};
std::vector<Line> rays;

// MOUSE MOVEMANT
glm::vec4 ray_clip = glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);
float lastX = 400, lastY = 300;

// PATH TO PROJECT
std::string shaderPath("resourses\\graphics\\shaders\\");
std::string Model3DPath("resourses\\graphics\\3Dmodel\\");

// ============= TESTING SOMTHING =================
	camera::CameraGame::UpAxis axisType = camera::CameraGame::UpAxis::Z;
	camera::CameraGame gameCam(glm::vec3(50.0f, 50.0f, 20.0f), axisType, glm::vec3(20.0f, 15.0f, 0.0f));
	camera::CameraCAD CADCam(glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)), glm::radians(50.0f), 150.0f, 0.3f);
	graphics::RotateShape3D rotateObject;
	
	// Functional object testing
	// -------------------------
	
	template<typename T>
	void fun(int i, T t) {
		cout << "template:\n";
		cout << "int: " << i << endl;
		cout << "T: " << t << endl;
	}
	void fun(int i, int k) {
		fun<>(i, k);
		cout << "not template:\n";
		cout << "int: " << i << endl;
		cout << "int: " << k << endl;
	}
	
	void functional_obj_test() {
		int f, j;
		double d;
		fun(f, j);
		fun(f, d);
	}
	
// ================================================
class TestWindowEvent {
public:
	TestWindowEvent(std::string name) : m_Name(name) {}
public:
	void scroll(double yoffset) {
		cout << m_Name << " >> scroll: " << yoffset << endl;
	}
	void mouseShowPos(double x, double y) {
		cout << m_Name << " >> X" << x << " Y" << y << endl;
	}
	void button(int button, int action, int mods) {
		cout << m_Name << " >> button: " << button << " " << "action: " << action << "mods: " << mods << endl;
	}
	void buttonPush(int button, int mods) {
		cout << m_Name << " >> button push: " << button << " mods: " << mods << endl;
	}
	void buttonRelese(int button, int mods) {
		cout << m_Name << " >> button relese: " << button << " mods: " << mods << endl;
	}
	void keyboardPush(int key, int scancode, int mods) {
		cout << m_Name << " >> key push: " << static_cast<char>(key) << " scancode: " << scancode <<  " mods: " << mods << endl;
	}
	void keyboardRelese(int key, int scancode, int mods) {
		cout << m_Name << " >> key relese: " << static_cast<char>(key) << " scancode: " << scancode <<  " mods: " << mods << endl;
	}
private:
	std::string m_Name;
};

void pos(int button, int action, int mods) {
	cout << "button: " << button << " " << "action: " << action << "mods: " << mods << endl;
}

int main(int argc, char* argv[]) {
	
	//functional_obj_test();
	//return 0;
	
	systemEvent::EventDispatcher *dispatchEvent = systemEvent::EventDispatcher::instance();
	camera::BaseCamera *camera = &CADCam;
	
	if(camera->getCameraType() == camera::BaseCamera::CameraType::CAD) {
		cout << "camera type is CAD" << std::endl;
	} else if (camera->getCameraType() == camera::BaseCamera::CameraType::GAME) {
		cout << "camera type is GAME" << std::endl;
	}
	
	gameCam.setMoveSpeed(15.0f);
	gameCam.setMouseSpeed(0.3f);
	
	// SET WINDOW CONTEXT
	// ------------------
	application::Window window("LearnOpenGL", SCR_WIDTH, SCR_HEIGHT);
	glfwMakeContextCurrent(window.getWindow());
	
	application::Window window2("Test Window", SCR_WIDTH / 2, SCR_HEIGHT / 2);
	
	window.activateContextWindow();

	TestWindowEvent windowEvent1(window.getWindowName());
	TestWindowEvent windowEvent2(window2.getWindowName());
	systemEvent::bindHandler(window, systemEvent::makeMouseMoveHandler(windowEvent1, TestWindowEvent::mouseShowPos));
	systemEvent::bindHandler(window2, systemEvent::makeMouseMoveHandler(windowEvent2, TestWindowEvent::mouseShowPos));

	systemEvent::bindHandler(window, systemEvent::makeMouseButtonPushHandler(windowEvent1, TestWindowEvent::buttonPush));
	systemEvent::bindHandler(window, systemEvent::makeMouseButtonReleseHandler(windowEvent1, TestWindowEvent::buttonRelese));

	systemEvent::bindHandler(window2, systemEvent::makeMouseButtonPushHandler(windowEvent2, TestWindowEvent::buttonPush));
	systemEvent::bindHandler(window2, systemEvent::makeMouseButtonReleseHandler(windowEvent2, TestWindowEvent::buttonRelese));

	systemEvent::bindHandler(window, systemEvent::makeMouseScrollHandler(windowEvent1, TestWindowEvent::scroll));
	systemEvent::bindHandler(window2, systemEvent::makeMouseScrollHandler(windowEvent2, TestWindowEvent::scroll));
	
	systemEvent::bindHandler(window, systemEvent::makeKeyboardPushHandler(windowEvent1, TestWindowEvent::keyboardPush));
	systemEvent::bindHandler(window, systemEvent::makeKeyboardReleseHandler(windowEvent1, TestWindowEvent::keyboardRelese));
	systemEvent::bindHandler(window2, systemEvent::makeKeyboardPushHandler(windowEvent2, TestWindowEvent::keyboardPush));
	systemEvent::bindHandler(window2, systemEvent::makeKeyboardReleseHandler(windowEvent2, TestWindowEvent::keyboardRelese));
	

	// CALLBACK FUNCTIONS
	// ------------------
    glfwSetFramebufferSizeCallback(window.getWindow(), framebuffer_size_callback);
	//glfwSetCursorPosCallback(window.getWindow(), mouse_callback);
	//glfwSetMouseButtonCallback(window.getWindow(), mouse_button_callback);
	//glfwSetScrollCallback(window.getWindow(), scroll_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }    

	// SET OPENGL STATES
	// -----------------
	glEnable(GL_DEPTH_TEST);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// some thing 
	// ---------------------------------------------
	//ShapeFactory &factoryShape = ShapeFactory::instance();

	// some 3D model
	// --------------
	graphics::Shader shader(shaderPath + "shader.vs", shaderPath + "shader.fs");
	graphics::Shader lightShader(shaderPath + "lightSource.vs", shaderPath + "lightSource.fs");
	std::shared_ptr<graphics::BaseShape> object1(ShapeFactory::instance().createShapeSTL(Model3DPath + "CROSS.STL"));
	std::shared_ptr<graphics::BaseShape> testQuat(ShapeFactory::instance().createShapeSTL(Model3DPath + "systemCoord.STL"));
	std::shared_ptr<graphics::BaseShape> lighter(ShapeFactory::instance().createShapeSTL(Model3DPath + "sphere.STL"));

	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 modelColor = glm::vec3(1.0f, 0.5f, 0.31f);
	
	// GENERETE LINE 
	rays.push_back(Line{glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 30.0f, 0.0f)});
	graphics::Shader primitiveShader(shaderPath + "primitiveShader.vs", shaderPath + "primitiveShader.fs");
	unsigned int lineVBO, lineVAO;
	glGenBuffers(1, &lineVBO);
	glGenVertexArrays(1, &lineVAO);
	glBindVertexArray(lineVAO); 
	glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
	glBufferData(GL_ARRAY_BUFFER, rays.size() * sizeof(Line), static_cast<void*>(rays.data()), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// SETUP DEAR IMGUI 
	// ------------------------
	const char* glsl_version = "#version 430";
	//IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window.getWindow(), true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	ImGui::StyleColorsDark();
	
	// FPS counter
	// -----------
	double startPoint = glfwGetTime();
	double step = 0.1;
	unsigned long frameCountPerStep = 0;
	double FPS = 0.0;
	
	// renderer loop 
	// -------------
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    while (!glfwWindowShouldClose(window.getWindow()))
    {
		if ((startPoint + step) > glfwGetTime()) {
			++frameCountPerStep;
		} else {
			
			FPS = frameCountPerStep * (1.0 / (glfwGetTime() - startPoint));
			startPoint = glfwGetTime();
			frameCountPerStep = 0;
		}
		
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
        
		// input
        // -----
        processInput(window.getWindow());
		
		glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
		//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		 
		// prerenderer setting 
		// -------------------
		//const float radius = 30.0f;
		float lightPosX = 0.0f;//sin(glfwGetTime()) * radius;
		float lightPosY = 0.0f;//sin(glfwGetTime() * 1.2f) * radius;
		float lightPosZ = 75.0f;//cos(glfwGetTime()) * radius;
		glm::vec3 lightPos = glm::vec3(lightPosX, lightPosY, lightPosZ);
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 700.0f);

//#define CAMERA_CAD
#ifdef CAMERA_CAD 
		glm::mat4 view = CADCam.getLookAtMatrix();
		glm::vec3 viewPos = CADCam.cameraPos();
		
#else
		// camera game
		//gameCam.setNewPos(gameCam.cameraPos() + gameCam.frontDir() * 0.01f);
		glm::mat4 view = gameCam.getLookAtMatrix();
		glm::vec3 viewPos = gameCam.cameraPos();
#endif
		// CALCULATE RAY DIRECTION FROM CAMERA
		// ------------------------ 
		glm::vec4 ray_eye = glm::inverse(projection) * ray_clip;
		ray_eye.z = -1.0f;
		ray_eye.w = 0.0f;
		glm::vec3 ray_wor = glm::vec3(glm::inverse(view) * ray_eye);
		ray_wor = glm::normalize(ray_wor);
		
		// render ImGui some stuff 
		// -----------------------
		// feed inputs to dear imgui, start new frame
			ImGui_ImplOpenGL3_NewFrame();	
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
		// render your GUI
			ImGui::Begin("Model Color");
			ImGui::Text("ray_clip: X:%f  Y:%f  Z:%f", ray_clip.x, ray_clip.y, ray_clip.z);
			ImGui::Text("ray_eye:  X:%f  Y:%f  Z:%f", ray_eye.x, ray_eye.y, ray_eye.z);
			ImGui::Text("ray_wor:  X:%f  Y:%f  Z:%f", ray_wor.x, ray_wor.y, ray_wor.z);
			static float color[4] = {0.064f, 0.926f, 0.495f, 1.0f};
			static glm::vec4 rayColor = glm::vec4{0.941f, 0.0, 1.0, 1.0f};
			ImGui::ColorEdit3("color 3D model", color);
			ImGui::ColorEdit3("color ray", &rayColor.x);
			ImGui::Text("FPS: %f", FPS);
			ImGui::End();
	

		// RENDERING OBJECTS
		// -----------------
		// draw 3d Model
		// ---------
		shader.use();
		glm::mat4 model = glm::mat4(1.0f);
		shader.setMat4f("model", model);
		shader.setMat4f("view", view);
		shader.setMat4f("projection", projection);
		shader.setVec3f("lightColor", lightColor);
		shader.setVec3f("modelColor", glm::vec3(color[0], color[1], color[2]));
		shader.setVec3f("lightPos", lightPos);
		shader.setVec3f("viewPos", viewPos);
		object1->draw();
		
		// TESTING QUATERNION 
		// ------------------
		shader.setMat4f("model", rotateObject.getMat4());
		shader.setVec3f("modelColor", glm::vec3(1.0f, 0.0f, 0.0f));
		testQuat->draw();
		
		// draw line 
		// ---------
		glBindVertexArray(lineVAO);
		float lengthRay = 10.0f;
		glm::vec3 endRayPos =  viewPos + ray_wor * lengthRay;
		if (pickWhenReleaseButton(glfwGetMouseButton(window.getWindow(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS))
			rays.push_back(Line{viewPos, endRayPos});
		rays[rays.size() - 1].beginLine = viewPos;
		rays[rays.size() - 1].endLine = endRayPos;
		glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
		glBufferData(GL_ARRAY_BUFFER, rays.size() * sizeof(Line), static_cast<void*>(rays.data()), GL_DYNAMIC_DRAW);

		primitiveShader.use();
		primitiveShader.setMat4f("model", glm::mat4(1.0f));
		primitiveShader.setMat4f("view", view);
		primitiveShader.setMat4f("projection", projection);
		primitiveShader.setVec3f("modelColor", glm::vec3(rayColor));
		glDrawArrays(GL_LINES, 0, rays.size() * 2);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		// draw lighter sphere
		lightShader.use();
		lightShader.setMat4f("model", glm::translate(glm::mat4(1.0f), lightPos));
		lightShader.setMat4f("view", view);
		lightShader.setMat4f("projection", projection);
		lighter->draw();
		// ShowDemoWindow draw exemple
			//ImGui::ShowDemoWindow();
		// Render dear imgui into screen
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwPollEvents();
        glfwSwapBuffers(window.getWindow());
    }

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
	return 0;
}

//////////////////////////////////////////////

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	
	ray_clip.x = (2 * xpos - SCR_WIDTH) / (float)SCR_WIDTH;
	ray_clip.y = -(2 * ypos - SCR_HEIGHT) / (float)SCR_HEIGHT;

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
	lastX = xpos;
	lastY = ypos;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		gameCam.turn(xoffset, yoffset);
		CADCam.turn(xoffset, yoffset);
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
		#define cameraDef gameCam
		rotateObject.rotate(xoffset, yoffset, cameraDef.frontDir(), cameraDef.rightDir(), cameraDef.upDir());
	}

}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
		
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		gameCam.move(camera::CameraGame::MoveDirection::FORWARD, deltaTime);
	}
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		gameCam.move(camera::CameraGame::MoveDirection::BACK, deltaTime);
	}
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		gameCam.move(camera::CameraGame::MoveDirection::LEFT, deltaTime);
	}
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		gameCam.move(camera::CameraGame::MoveDirection::RIGHT, deltaTime);
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
    glViewport(0, 0, width, height);

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{

}

//////////////////////////////////////////////