#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include "World.hpp"
#include "Sprite3.hpp"
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 400.0f; // Center of the screen
float lastY = 300.0f; // Center of the screen
bool firstMouse = true;
float sensitivity = 0.1f;


glm::vec3 viewVector = glm::vec3(0.0f, 0.0f, -5.0f);
glm::vec3* viewVectorPointer = &viewVector;



void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}




const char* readShaderFromFile(const std::string& filePath) {
	std::ifstream shaderFile(filePath); // Open the file
	if (!shaderFile) { // Check if the file was successfully opened
		std::cerr << "Failed to open shader file: " << filePath << std::endl;
		return nullptr; // Return nullptr on failure
	}

	std::stringstream shaderStream;
	shaderStream << shaderFile.rdbuf(); // Read the file content into the stream
	std::string shaderSource = shaderStream.str(); // Convert the stream into a string

	// Allocate memory to store the shader source
	char* shaderCode = new char[shaderSource.size() + 1];
	std::copy(shaderSource.begin(), shaderSource.end(), shaderCode);
	shaderCode[shaderSource.size()] = '\0'; // Null-terminate the string

	return shaderCode; // Return the shader source as a const char*
}



GLsizei glwidth = 1200;
GLsizei glheight = 720;



struct MYVector4
{
	float a;
	float b;
	float c;
	float d;

	MYVector4(float a, float b, float c, float d) : a(a), b(b), c(c), d(d) {};

};


const char* vertexShaderSource = readShaderFromFile("C:\\Users\\ADMIN\\source\\repos\\openglsampleapp\\openglsampleapp\\shader\\vertexShader.glsl");

const char* fragmentShaderSource = readShaderFromFile("C:\\Users\\ADMIN\\source\\repos\\openglsampleapp\\openglsampleapp\\shader\\FargmentShader.glsl");


void changeVec4DataUniform(GLuint Shaderprogram,const GLchar* name, MYVector4* vec4) {

	int vertexColorLocation = glGetUniformLocation(Shaderprogram, name);
	glUniform4f(vertexColorLocation, vec4->a,vec4->b,vec4->c,vec4->d);


}




void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1200, 720, "learn", NULL, NULL);

	if (window == nullptr) {
		std::cout << "failed to create glfw window";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	//IMgui 

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");



	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	//---------------------------------------------------------------------------------------
	
	
	std::vector<float> vertices = {
		-0.5f, -0.5f, -0.5f,  
		 0.5f, -0.5f, -0.5f, 
		 0.5f,  0.5f, -0.5f,  
		 0.5f,  0.5f, -0.5f,  
		-0.5f,  0.5f, -0.5f, 
		-0.5f, -0.5f, -0.5f,  

		-0.5f, -0.5f,  0.5f,  
		 0.5f, -0.5f,  0.5f,  
		 0.5f,  0.5f,  0.5f,  
		 0.5f,  0.5f,  0.5f, 
		-0.5f,  0.5f,  0.5f,  
		-0.5f, -0.5f,  0.5f,  

		-0.5f,  0.5f,  0.5f,  
		-0.5f,  0.5f, -0.5f,  
		-0.5f, -0.5f, -0.5f,  
		-0.5f, -0.5f, -0.5f,  
		-0.5f, -0.5f,  0.5f,  
		-0.5f,  0.5f,  0.5f,  

		 0.5f,  0.5f,  0.5f,  
		 0.5f,  0.5f, -0.5f,  
		 0.5f, -0.5f, -0.5f,  
		 0.5f, -0.5f, -0.5f,  
		 0.5f, -0.5f,  0.5f,  
		 0.5f,  0.5f,  0.5f,  

		-0.5f, -0.5f, -0.5f,  
		 0.5f, -0.5f, -0.5f,  
		 0.5f, -0.5f,  0.5f,  
		 0.5f, -0.5f,  0.5f,  
		-0.5f, -0.5f,  0.5f,  
		-0.5f, -0.5f, -0.5f,  

		-0.5f,  0.5f, -0.5f,  
		 0.5f,  0.5f, -0.5f,  
		 0.5f,  0.5f,  0.5f,  
		 0.5f,  0.5f,  0.5f,  
		-0.5f,  0.5f,  0.5f,  
		-0.5f,  0.5f, -0.5f,  	};

	Shader shader("C:\\Users\\ADMIN\\source\\repos\\openglsampleapp\\openglsampleapp\\shader\\vertexShader.glsl", "C:\\Users\\ADMIN\\source\\repos\\openglsampleapp\\openglsampleapp\\shader\\FargmentShader.glsl");

	World world;

	Sprite3 cube(vertices, shader);

	cube.modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.9f, 1.0f));
	
	for (int i = 0; i < 10; i++)

	{
		for (int x = 0; x < 10; x++) {

		Sprite3* cube = new Sprite3(vertices, shader);
		world.addSprite(cube);
		cube->setposition(i * 1.0f, 0.0f, x*1.0f);
		}


	}


	world.addSprite(&cube);

	






	float x = 0.0f;
	glEnable(GL_DEPTH_TEST);

	cube.updateRotation(40.5f);
	float y = 0.0f;
	int glPositionY = 0;

	glfwSetCursorPosCallback(window, mouse_callback);
	

	// tell GLFW to capture our mouse
	
	


	while (!glfwWindowShouldClose(window))

	{
		//processing input -----------------------------------------------------------
		processInput(window);




		//-----------------------------------------------------------
		



		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Create ImGui window
		ImGui::Begin("Hello, world!",nullptr, ImGuiWindowFlags_NoMove);
		ImGui::Text("This is some useful text.");
		

		if (ImGui::SliderFloat("x", &x, -1.0f, 2.0f))
		{
			

			cube.setposition(x,NULL,NULL);
			
		};
		if (ImGui::SliderFloat("ydss", &y, -1.0f, 2.0f))
		{
			
			cube.setposition(NULL,y,NULL);

		};
		if (ImGui::SliderInt("y", &glPositionY, 0, 1000))
		{

			glViewport(static_cast<GLint>(glPositionY),0,glwidth,glheight);

		};
		
		
		ImGui::End();

		// Rendering
		ImGui::Render();

		

		
		




		//rendering-----------------------------------------------------------
		//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(5.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		glClearColor(0.2f, 0.1f, 0.4f, 0.2f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view = glm::translate(glm::mat4(1.0f), viewVector);
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		//view = glm::rotate(view, glm::radians(99.0f), glm::vec3(0.0f, 0.3f, 0.4f));
		float aspectRatio = static_cast<float>(glwidth) / static_cast<float>(glheight);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
		
		
		world.render(view, projection);



		//-----------------------------------------------------------

		//swapping 
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}




	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	glwidth = static_cast<GLint>(width);
	glheight = static_cast<GLint>(height);

}
void processInput(GLFWwindow* window)
{
	
	const float cameraSpeed = 0.05f; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void processInput1(GLFWwindow* window)


{

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		viewVectorPointer->x += 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		viewVectorPointer->x -= 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
	{
		viewVectorPointer->z += 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
	{
		viewVectorPointer->z -= 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		viewVectorPointer->y += 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		viewVectorPointer->y -= 0.1f;
	}
}
;
