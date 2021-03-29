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
#include "camera/camera.h"

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
glm::vec3 cameraFront  = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// LINE PRINMITIVE
struct Line {
	glm::vec3 beginLine;
	glm::vec3 endLine;
};
std::vector<Line> rays;

// KEYS MOVEMANT
float camYawDigree = 0.0f;
float camPitchDigree = 90.0f;
float camRadius = 80.0f;

// MOUSE MOVEMANT
glm::vec4 ray_clip = glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);
float lastX = 400, lastY = 300;
float yaw = -90.0f, pitch = 0.0f;

// state button 
bool mousMidleButton = false;

// PATH TO PROJECT
std::string shaderPath("e:\\project\\MyProject\\resourses\\graphics\\shaders\\");
std::string Model3DPath("e:\\project\\MyProject\\resourses\\graphics\\3Dmodel\\");

// ============= TESTING SOMTHING =================
	camera::CameraGame gameCam(glm::vec3(30.0f, 30.0f, 30.0f), glm::vec3(0.0f, 0.0f, 0.0f));
// ================================================

int main(int argc, char* argv[]) {

	gameCam.setMoveSpeed(10.0f);
	gameCam.setMouseSpeed(0.3f);
	// glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
	// SET WINDOW CONTEXT
	// ------------------
	glfwMakeContextCurrent(window);
	
	// CALLBACK FUNCTIONS
	// ------------------
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);

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
	std::shared_ptr<graphics::BaseShape> object1(ShapeFactory::instance().createShapeSTL(Model3DPath + "CROSS.STL"));

	glm::vec3 lightColor = glm::vec3(1.0f);
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
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	ImGui::StyleColorsDark();
	
	// renderer loop 
	// -------------
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    while (!glfwWindowShouldClose(window))
    {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
        
		// input
        // -----
        processInput(window);
		
		glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
		//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// prerenderer setting 
		// -------------------
		//const float radius = 30.0f;
		float lightPosX = -10.0f;//sin(glfwGetTime()) * radius;
		float lightPosY = 30.0f;//sin(glfwGetTime() * 1.2f) * radius;
		float lightPosZ = -5.0f;//cos(glfwGetTime()) * radius;
		glm::vec3 lightPos = glm::vec3(lightPosX, lightPosY, lightPosZ);
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 300.0f);

//#define CAMERA_CAD
#ifdef CAMERA_CAD
		// camera cad/cam
		glm::vec3 viewPos = pointOnSphear(camRadius, camYawDigree, camPitchDigree);
		glm::vec3 frontDirection = glm::normalize(glm::vec3(0.0f, 0.0f, 0.0f) - viewPos);
		glm::mat4 view = glm::lookAt(viewPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
#else
		// camera game
		glm::vec3 viewPos = gameCam.position();
		glm::vec3 frontDirection = gameCam.frontDir();
		glm::mat4 view = gameCam.getLookAtMatrix();
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
		
		// draw line 
		// ---------
		glBindVertexArray(lineVAO);
		float lengthRay = 10.0f;
		glm::vec3 endRayPos =  viewPos + ray_wor * lengthRay;
		if (pickWhenReleaseButton(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS))
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
		
		// ShowDemoWindow draw exemple
			//ImGui::ShowDemoWindow();
		// Render dear imgui into screen
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwPollEvents();
        glfwSwapBuffers(window);
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

	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);
	if (state != GLFW_PRESS)
		return;
		
	gameCam.turn(xoffset, yoffset);

	const float sensitivity = 0.3f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	
	camYawDigree -= xoffset;
	camPitchDigree += yoffset;
	if(camPitchDigree < 0.0f) camPitchDigree = 1.0f;
	if(camPitchDigree > 180.0f) camPitchDigree = 179.0f;

}

void processInput(GLFWwindow *window)
{
	const float cameraSpeed = 10.0f * deltaTime; // adjust accordingly
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
	
	// find angel on the spher
	const float sensitivity = 10.0f;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) 
		camYawDigree += cameraSpeed * sensitivity;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		camYawDigree -= cameraSpeed * sensitivity;	
	
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)  {
		camPitchDigree += (cameraSpeed * sensitivity);
		if(camPitchDigree > 180.0f) camPitchDigree = 179.9f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)  {
		camPitchDigree -= (cameraSpeed * sensitivity);
		if(camPitchDigree < 0.0f) camPitchDigree = 0.1f;
	}
	
	if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
		camRadius += cameraSpeed * sensitivity;
	if (glfwGetKey(window, GLFW_KEY_KP_ADD ) == GLFW_PRESS)
		camRadius -= cameraSpeed * sensitivity;
		
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
    camRadius -= (float)yoffset * 10.0f;
	if(camRadius < 0)
		camRadius = 0.1f;
    
}

//////////////////////////////////////////////