#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <memory>

#include "../parserXML/src/smartBuffer.h"
#include "../parserXML/src/lexerXML.h"
#include "../parserXML/parserXML.h"
#include "../parserXML/src/exceptionParserXML.h"

using std::cout;
using std::cin;
using std::endl;

// MACROS FUNCTIONS
#define PRINT(p) std::cout << #p << " => " << p
#define PAUSE_MSG(msg) std::cout << msg << endl; std::cin.get()

//////////////////////////////////////////////
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
//////////////////////////////////////////////


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
	std::ofstream fout("e:\\project\\MyProject\\resourses\\testParser.xml");
	parserXML::ParserXML parser("e:\\project\\MyProject\\resourses\\langs.model.xml");
	parser.parse();
	printAllDoc(parser.getRootElement(), fout, 0);
}



int main(int argc, char* argv[]) {
	
	// glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }    

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();

	
	/*
	try {
		//test_parserXML();
		std::cout << "good job man" << endl;
	} catch(parserXML::ExceptionParserXML &except) {
		cout << except.what() << endl;
	}
	*/
	return 0;
}

//////////////////////////////////////////////
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
//////////////////////////////////////////////