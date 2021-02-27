#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS // use for lib glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <memory>
#include <cmath>

#include "parserXML/smartBuffer.h"
#include "parserXML/lexerXML.h"
#include "parserXML/parserXML.h"
#include "parserXML/exceptionParserXML.h"
#include "graphics/shader/shader.h"
#include "graphics/stb/stb_image.h"
#include "parserSTL/parserSTL.h"

#include "graphics/imGui/imgui.h"
#include "graphics/imGui/imgui_impl_glfw.h"
#include "graphics/imGui/imgui_impl_opengl3.h"

using std::cout;
using std::cin;
using std::endl;

// MACROS FUNCTIONS
#define PRINT(p) std::cout << #p << " => " << p
#define PAUSE_MSG(msg) std::cout << msg << endl; std::cin.get()
#define PRINTVEC4(vec4) cout << "vec4 => x:" << vec4.x << " y:" << vec4.y << " z:" << vec4.z << " w:" << vec4.w << endl


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

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;

//////////////////////////////////////////////
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront  = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// KEYS MOVEMANT
float camYawDigree = 0.0f;
float camPitchDigree = 90.0f;
float camRadius = 80.0f;

// MOUSE MOVEMANT
bool firstMouse = true;
float lastX = 400, lastY = 300;
float yaw = -90.0f, pitch = 0.0f;

// state button 
bool mousMidleButton = false;

// PATH TO PROJECT
std::string pathName("e:\\project");

int main(int argc, char* argv[]) {
	
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

	// light source (lamp sphere)
	// --------------------------
	graphics::ParserSTL lampSphere(pathName + "\\MyProject\\resourses\\graphics\\3Dmodel\\Perfect_sphere.STL");
	std::string vertexLightSrc(pathName + "\\MyProject\\resourses\\graphics\\shaders\\lightSource.vs");
	std::string fragmentLightSrc(pathName + "\\MyProject\\resourses\\graphics\\shaders\\lightSource.fs");
	graphics::Shader LampShader(vertexLightSrc, fragmentLightSrc);
	unsigned int lampVBO, lampVAO;
	glGenBuffers(1, &lampVBO);
	glGenVertexArrays(1, &lampVAO);
	glBindVertexArray(lampVAO);
	glBindBuffer(GL_ARRAY_BUFFER, lampVBO);
	glBufferData(GL_ARRAY_BUFFER, lampSphere.sizeMashBuffer(), lampSphere.getVoidPtr(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, lampSphere.sizeElement(), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// some 3D model
	// --------------
	graphics::ParserSTL modelCamera(pathName + "\\MyProject\\resourses\\graphics\\3Dmodel\\Camera.STL");
	std::string vertexShaderSrc(pathName + "\\MyProject\\resourses\\graphics\\shaders\\shader.vs");
	std::string fragmentShaderSrc(pathName + "\\MyProject\\resourses\\graphics\\shaders\\shader.fs");
	graphics::Shader shader(vertexShaderSrc, fragmentShaderSrc);
	unsigned int cameraModelVBO, cameraModelVAO;
	glGenBuffers(1, &cameraModelVBO);
	glGenVertexArrays(1, &cameraModelVAO);
	glBindVertexArray(cameraModelVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cameraModelVBO);
	glBufferData(GL_ARRAY_BUFFER, modelCamera.sizeMashBuffer(), modelCamera.getVoidPtr(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, modelCamera.sizeElement(), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, modelCamera.sizeElement(), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glm::vec3 lightColor = glm::vec3(1.0f);
	glm::vec3 modelColor = glm::vec3(1.0f, 0.5f, 0.31f);
	
	// SETUP DEAR IMGUI 
	// ------------------------
	const char* glsl_version = "#version 430";
	IMGUI_CHECKVERSION();
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
		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// imgui daw loop
		// --------------
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		// prerenderer setting 
		// -------------------
		const float radius = 30.0f;
		float camX = sin(glfwGetTime()) * radius;
		//float camY = sin(glfwGetTime() * 1.2f) * radius;
		float camZ = cos(glfwGetTime()) * radius;
		glm::vec3 lightPos = glm::vec3(camX, 0.0f, camZ);
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 300.0f);
		
		// camera cad/cam
		glm::mat4 view = glm::lookAt(pointOnSphear(camRadius, camYawDigree, camPitchDigree), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::vec3 viewPos = pointOnSphear(camRadius, camYawDigree, camPitchDigree);
		
		// camera game
		//glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, glm::vec3(0.0f, 1.0f, 0.0f));
		//glm::vec3 viewPos = cameraPos;
		
		// rendering commands here
		// -----------------------
		shader.use();
		glBindVertexArray(cameraModelVAO);
		glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(0.3f, 0.3f, 0.3f));
		shader.setMat4f("model", model);
		shader.setMat4f("view", view);
		shader.setMat4f("projection", projection);
		shader.setVec3f("lightColor", lightColor);
		shader.setVec3f("modelColor", modelColor);
		shader.setVec3f("lightPos", lightPos);
		shader.setVec3f("viewPos", viewPos);
		glDrawArrays(GL_TRIANGLES, 0, modelCamera.countElement());
		
		// lamp sphere draw
		// ----------------
		LampShader.use();
		glBindVertexArray(lampVAO);
		LampShader.setMat4f("model", glm::translate(glm::mat4(1.0f), lightPos));
		LampShader.setMat4f("projection", projection);
		LampShader.setMat4f("view", view);
		glDrawArrays(GL_TRIANGLES, 0, lampSphere.countElement());
		
		// render ImGui some stuff 
		// -----------------------
		ImGui::Begin("Demo window");
		ImGui::Button("Hello!");
		ImGui::End();
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
	
	glDeleteVertexArrays(1, &cameraModelVAO);
	glDeleteVertexArrays(1, &lampVAO);
    glDeleteBuffers(1, &cameraModelVBO);
    glDeleteBuffers(1, &lampVBO);
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();

	/*try {
		//test_parserXML();
		std::cout << "good job man" << endl;
	} catch(parserXML::ExceptionParserXML &except) {
		cout << except.what() << endl;
	}
	*/
	return 0;
}

//////////////////////////////////////////////

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {

	
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	
	if (firstMouse) // initially set to true
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
	lastX = xpos;
	lastY = ypos;
	
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);
	if (state != GLFW_PRESS)
		return;

	
	const float sensitivity = 0.3f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	
	camYawDigree -= xoffset;
	camPitchDigree += yoffset;
	if(camPitchDigree < 0.0f) camPitchDigree = 1.0f;
	if(camPitchDigree > 180.0f) camPitchDigree = 179.0f;
	
	yaw   += xoffset;
	pitch += yoffset; 
	if(pitch > 89.0f)
	  pitch =  89.0f;
	if(pitch < -89.0f)
	  pitch = -89.0f;
	
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);

}

void processInput(GLFWwindow *window)
{
	const float cameraSpeed = 6.0f * deltaTime; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	
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