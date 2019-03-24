// g++ code4.cpp -lglfw -lGL -lGLEW

#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>

#define ASSERT(x) if(!(x)) std::cerr<<"Error "; 
#define GLCall(x) GLClearError(); x;ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError(){
	while(glGetError() != GL_NO_ERROR);
}
bool GLLogCall(const char* function, const char* file, int line){
	while(GLenum error = glGetError()){
		std::cout<<"[OpenGL Error] (0x"<<std::hex <<error<<"): "
			<<function<<" "<<file<<":"<< line <<std::endl;
		return false;
	}
	return true;
}

unsigned int compileShader(unsigned int shader,const char* src,int shaderType){
	unsigned int shaderId = glCreateShader(shaderType);
	GLCall(glShaderSource(shaderId, 1, &src, nullptr));
	GLCall(glCompileShader(shaderId));
	int result;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
	if(!result){
		int length;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH,&length);
		char* message = (char*)alloca(length* sizeof(char));
		glGetShaderInfoLog(shaderId, length, &length, message);
		std::cout<<"Failed to Compile";
		if(shaderType==GL_VERTEX_SHADER) std::cout<<" GL_VERTEX_SHADER ";
		else if(shaderType==GL_FRAGMENT_SHADER) std::cout<<" GL_FRAGMENT_SHADER ";
		std::cout<<std::endl;
		std::cout<<message<<std::endl;
	}
	glAttachShader(shader,shaderId);
	
	return shaderId;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
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
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSwapInterval(1);

	if(glewInit() != GLEW_OK)
		std::cout<<"GLEW Error!"<<std::endl;
	
	std::cout<<glGetString(GL_VERSION)<<std::endl;
	
	{
		float positions[] ={
			//positions
			 1.0f,  1.0f, //top right
			 1.0f, -1.0f, //bottom right
			-1.0f, -1.0f, //bottom left
			-1.0f,  1.0f  //top left
		};
		
		unsigned int indices[]={
			0, 1, 2,
			2, 3, 0
		};

		unsigned int vao;
		GLCall(glGenVertexArrays(1, &vao));
		GLCall(glBindVertexArray(vao));
		
		unsigned int buffer;
		GLCall(glGenBuffers(1, &buffer));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
		GLCall(glBufferData(GL_ARRAY_BUFFER, 4*2*sizeof(float), positions, GL_STATIC_DRAW));
		
		GLCall(glEnableVertexAttribArray(0));
		GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0));

		unsigned int ibo;
		GLCall(glGenBuffers(1, &ibo));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(unsigned int), indices, 
			GL_STATIC_DRAW));
		
		std::string vertexShaderSrc =
			"#version 330 core\n"
			"layout(location = 0) in vec4 position;\n"
			"void main(){\n"
			"	gl_Position = position;\n"
			"};\n";
		std::string fragmentShaderSrc =
			"#version 330 core\n"
			"out vec4 color;\n"
			"void main(){\n"
			"	color = vec4(0.0, 0.0, 1.0, 1.0);\n"
			"};\n";			
		unsigned int shader = glCreateProgram();
		unsigned int vs = compileShader(shader, vertexShaderSrc.c_str(), GL_VERTEX_SHADER);
		unsigned int fs = compileShader(shader, fragmentShaderSrc.c_str(), GL_FRAGMENT_SHADER);
		GLCall(glLinkProgram(shader));
		glValidateProgram(shader);
		GLCall(glUseProgram(shader));

		while(!glfwWindowShouldClose(window)){
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}
	
	glfwTerminate();
	return 0;
}
