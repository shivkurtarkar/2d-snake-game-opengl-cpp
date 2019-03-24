// g++ code3.cpp -lglfw -lGL -lGLEW

//#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>

#define ASSERT(x) if(!(x)) std::cerr<<"Error "; 
#define GLCall(x) GLClearError(); x;ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError(){
	while(glGetError() != GL_NO_ERROR);
}
bool GLLogCall(const char* function, const char* file, int line){
	while(GLenum error = glGetError()){
		std::cout<<"[OPenGL Error] (0x"<<std::hex <<error<<"): "
			<<function<<" "<<file<<":"<< line <<std::endl;
		return false;
	}
	return true;
}

int main(){
	GLFWwindow* window;
	if(!glfwInit())
		return -1;
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(640,640, "snake game",NULL, NULL);
	if(!window){
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window);
	//set other window properties
	//...

//	if(glewInit() != GLEW_OK)
//		std::cout<<"GLEW Error!"<<std::endl;
	
	std::cout<<glGetString(GL_VERSION)<<std::endl;
	
	{
		while(!glfwWindowShouldClose(window)){
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

	}
	glfwTerminate();
	return 0;
}
