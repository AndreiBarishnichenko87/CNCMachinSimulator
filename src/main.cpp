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
#include "graphics/shapeManipulators/rotateShape3D.h"
#include "application/window/window.h"

#include "application/eventSystem/eventDespatcher.h"

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

// LINE PRINMITIVE
struct Line {
	glm::vec3 beginLine;
	glm::vec3 endLine;
};
std::vector<Line> rays;

// MOUSE MOVEMANT
glm::vec4 ray_clip = glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);

// PATH TO PROJECT
std::string shaderPath("resourses\\graphics\\shaders\\");
std::string Model3DPath("resourses\\graphics\\3Dmodel\\");

// ============= TESTING SOMTHING =================
	graphics::RotateShape3D rotateObject;

	// Functional object testing
	// -------------------------

	#include "camera/camera.h"
	#include "camera/cameraManipulator.h"
	
	glm::mat4 getLookAtMatrix(camera::CameraGeometryData* camera) {
		return glm::lookAt(camera->getPos(), camera->getPos() + camera->getFrontDir(), camera->getUpDir());
	}
	
	#define STEP_MOVE 0.5f
	#define ROTE_SENS 0.1f
	
	class TestRotManip {
	public:
		TestRotManip(camera::CameraGeometryData *camera, camera::CameraRotateManipulator *manipulator)
			: m_Camera(camera), m_Manipulator(manipulator) { }
	public:
		void mouseMove(double x, double y) {
			static float prevX = x;
			static float prevy = y;
			float xoffset = x - prevX; float yoffset = prevy - y;
			if(yoffset > 0.0) {
				m_Manipulator->rotateUp(m_Camera, yoffset * ROTE_SENS);
				m_Manipulator->rotateClockWise(m_Camera, yoffset * ROTE_SENS);
			}
			if(yoffset < 0.0) {
				m_Manipulator->rotateDown(m_Camera, abs(yoffset) * ROTE_SENS);
				m_Manipulator->rotatecounterClockWise(m_Camera, abs(yoffset) * ROTE_SENS);
			}
			if(xoffset > 0.0) {
				m_Manipulator->rotateRight(m_Camera, xoffset * ROTE_SENS);
			}
			if(xoffset < 0.0) {
				m_Manipulator->rotateLeft(m_Camera, abs(xoffset) * ROTE_SENS);
			}
			prevX = x; prevy = y;
		}
	private:
		camera::CameraGeometryData *m_Camera;
		camera::CameraRotateManipulator *m_Manipulator;
	};

	class TestCameraManip {
	public:
		TestCameraManip(camera::CameraGeometryData *camera, camera::CameraMoveManipulator *manipulator, int frontKey, int backKey, int leftKey, int rightKey)
			: 	m_Camera(camera), m_ManipulatorMove(manipulator), m_KeyFront(frontKey), m_KeyBack(backKey), m_KeyLeft(leftKey), m_KeyRight(rightKey) { }
	public:
		void keyboardPush(int key, int scancode, int mods) {
			cout << "Manipulator >> key push: " << static_cast<char>(key) << " scancode: " << scancode <<  " mods: " << mods << endl;
			if(key == m_KeyFront) {
				m_ManipulatorMove->moveFront(m_Camera, STEP_MOVE);
			} else if(key == m_KeyBack) {
				m_ManipulatorMove->moveBack(m_Camera, STEP_MOVE);
			} else if(key == m_KeyLeft) {
				m_ManipulatorMove->moveLeft(m_Camera, STEP_MOVE);
			} else if(key == m_KeyRight) {
				m_ManipulatorMove->moveRight(m_Camera, STEP_MOVE);
			}
		}

		void updateCamera(GLFWwindow *window) {
			if(GLFW_PRESS == glfwGetKey(window, m_KeyFront)) {
				m_ManipulatorMove->moveFront(m_Camera, STEP_MOVE); } 
			if(GLFW_PRESS == glfwGetKey(window, m_KeyBack)) {
				m_ManipulatorMove->moveBack(m_Camera, STEP_MOVE); }
			if(GLFW_PRESS == glfwGetKey(window, m_KeyLeft)) {
				m_ManipulatorMove->moveLeft(m_Camera, STEP_MOVE); }
			if(GLFW_PRESS == glfwGetKey(window, m_KeyRight)) {
				m_ManipulatorMove->moveRight(m_Camera, STEP_MOVE); }
		}
	public:
		camera::CameraGeometryData* getCameraPtr() { return m_Camera; }
		void changeManipulator(camera::CameraMoveManipulator *manipulator) { m_ManipulatorMove = manipulator; }
	private:
		camera::CameraGeometryData *m_Camera;
		camera::CameraMoveManipulator *m_ManipulatorMove;
		int m_KeyFront = -1; 
		int m_KeyBack = -1; 
		int m_KeyLeft = -1; 
		int m_KeyRight = -1;
	};
	
	glm::vec3 cameraRayCasting(const glm::vec3 &cameraPos, float NDC_x, float NDC_y, const glm::mat4 &projecMat4, const glm::mat4 &viewMat4);
	
int main(int argc, char* argv[]) {

	//return 0;

	// SET WINDOW CONTEXT
	// ------------------
	application::Window window("LearnOpenGL", SCR_WIDTH, SCR_HEIGHT);
	//application::Window window2("Test Window", SCR_WIDTH / 2, SCR_HEIGHT / 2);
	
	glfwMakeContextCurrent(window.getWindow());
	window.activateContextWindow();
	//glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Testing manipualtor CameraGeometryData
	// --------------------------

		glm::vec3 camPosition(0.0f, 0.0f, 0.0f);
		camera::CameraGeometryData cam(camPosition, camera::WorldUpVector::Y);
		
		camera::CameraMoveManipulator *manipulatorCamMove = new camera::MoveAlongCameraAxis;
		TestCameraManip testCamManipulators(&cam, manipulatorCamMove, int('W'), int('S'), int('A'), int('D'));
		
		//camera::CameraMoveManipulator *manipulatorCamUpWorld = new camera::MoveInCameraUpWorld;
		//TestCameraManip testCamManipulators(&cam, manipulatorCamUpWorld, int('W'), int('S'), int('A'), int('D'));
		
		camera::CameraRotateGameWay rotCamGame(95.0f);
		TestRotManip testRoteManipulator(&cam, &rotCamGame);
		
		//camera::CameraRotateCADWay rotateCAD(cam.getUpWorldVec_t());
		//camera::RotateDecoratorCAD moveRoteCAD(rotateCAD, glm::vec3(0.0f, 0.0f, 0.0f));
		//TestRotManip testRoteManipulator(&cam, &moveRoteCAD);
		
		systemEvent::bindHandler(window, systemEvent::makeMouseMoveHandler(testRoteManipulator, TestRotManip::mouseMove));
		//systemEvent::bindHandler(window, systemEvent::makeKeyboardPushHandler(testCamManipulators, TestCameraManip::keyboardPush));
		//systemEvent::bindHandler(window, systemEvent::makeKeyboardRepeatHandler(testCamManipulators, TestCameraManip::keyboardPush));

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


	// Camera data LookAtMatrix
		testCamManipulators.updateCamera(window.getWindow());
		glm::vec3 viewPos = cam.getPos();
		glm::mat4 view = getLookAtMatrix(&cam);
			
		// CALCULATE RAY DIRECTION FROM CAMERA
		// ------------------------ 
		double ndc_x, ndc_y;
		glfwGetCursorPos(window.getWindow(), &ndc_x, &ndc_y);
		ndc_x = (ndc_x * 2.0f - SCR_WIDTH) / SCR_WIDTH;
		ndc_y = (SCR_HEIGHT - ndc_y * 2.0f) / SCR_HEIGHT;
		glm::vec3 rayCastDir = cameraRayCasting(cam.getPos(), ndc_x, ndc_y, projection, view);

		// render ImGui some stuff 
		// -----------------------
		// feed inputs to dear imgui, start new frame
			ImGui_ImplOpenGL3_NewFrame();	
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
		// render your GUI
			ImGui::Begin("Model Color");
			ImGui::Text("ray_wor:  X:%f  Y:%f  Z:%f", rayCastDir.x, rayCastDir.y, rayCastDir.z);
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
		glm::vec3 endRayPos =  viewPos + rayCastDir * lengthRay;
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

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
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

glm::vec3 cameraRayCasting(const glm::vec3 &cameraPos, float NDC_x, float NDC_y, const glm::mat4 &projecMat4, const glm::mat4 &viewMat4) {
	//assert(NDC_x >= -1.0f && NDC_x <= 1.0f && "normalize device coordinat X out of range");
	//assert(NDC_y >= -1.0f && NDC_y <= 1.0f && "normalize device coordinat X out of range");

	// 3d Normalised Device Coordinates
	// 4d Homogeneous Clip Coordinates
	// 4d Eye (Camera) Coordinates
	// 4d World Coordinates

	glm::vec4 ray_eye = glm::inverse(projecMat4) * glm::vec4(NDC_x, NDC_y, -1.0, 1.0);
	ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0f, 0.0f);

	return glm::normalize(glm::vec3(glm::inverse(viewMat4) * ray_eye));
}