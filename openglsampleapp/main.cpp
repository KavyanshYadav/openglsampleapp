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
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Sprite3.hpp"
float y = 0.0f;

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

std::pair<std::vector<Vertex>, std::vector<GLuint>> loadModel(const std::string& path) {
	std::cout << "ad";
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	try{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	aiMesh* mesh = scene->mMeshes[0];  // Assuming only one mesh for simplicity

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		aiVector3D pos = mesh->mVertices[i];
		aiVector3D normal = mesh->mNormals[i];
		
		vertex.position = glm::vec3(pos.x, pos.y, pos.z);
		vertex.normal = glm::vec3(normal.x, normal.y, normal.z);

		vertices.push_back(vertex);
	}
	

	


	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		const aiFace& face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}
	std::cout << "nemss" << indices.size();

	}
	catch (const std::exception&)
	{
	}
	return  std::make_pair(vertices, indices);;
	

	
}



GLuint createMesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices) {
	GLuint VAO, VBO, EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO); // Index Buffer Object

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);


	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);

	return VAO;
}

float laodmodelz = 90.0f;

void renderModel(GLuint VAO, GLuint numVertices, Shader& shader) {
	shader.use();
	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,0.0f,-1.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(270.0f), glm::vec3(1.0f,0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(laodmodelz), glm::vec3(0.0f, 0.0f, 1.0f));

	shader.setMat4("model", modelMatrix);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, numVertices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}





glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 400.0f; // Center of the screen
float lastY = 300.0f; // Center of the screen
bool firstMouse = true;
float sensitivity = 0.1f;


glm::vec3 viewVector = glm::vec3(-3.0f, -4.2f, -27.0f);
glm::vec3* viewVectorPointer = &viewVector;

glm::vec3 rotatevec = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3* rotatevecPointer = &rotatevec;

void processInput(GLFWwindow* window);


//const char* readShaderFromFile(const std::string& filePath) {
//	std::ifstream shaderFile(filePath); // Open the file
//	if (!shaderFile) { // Check if the file was successfully opened
//		std::cerr << "Failed to open shader file: " << filePath << std::endl;
//		return nullptr; // Return nullptr on failure
//	}
//
//	std::stringstream shaderStream;
//	shaderStream << shaderFile.rdbuf(); // Read the file content into the stream
//	std::string shaderSource = shaderStream.str(); // Convert the stream into a string
//
//	// Allocate memory to store the shader source
//	char* shaderCode = new char[shaderSource.size() + 1];
//	std::copy(shaderSource.begin(), shaderSource.end(), shaderCode);
//	shaderCode[shaderSource.size()] = '\0'; // Null-terminate the string
//
//	return shaderCode; // Return the shader source as a const char*
//}



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


//const char* vertexShaderSource = readShaderFromFile("C:\\Users\\ADMIN\\source\\repos\\openglsampleapp\\openglsampleapp\\shader\\vertexShader.glsl");
//
//const char* fragmentShaderSource = readShaderFromFile("C:\\Users\\ADMIN\\source\\repos\\openglsampleapp\\openglsampleapp\\shader\\FargmentShader.glsl");
//

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


	glViewport(100, 0, glwidth, glheight);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	//---------------------------------------------------------------------------------------
	
	
	std::vector<float> vertices = {
		// positions          // normals
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	Shader shader("C:\\Users\\ADMIN\\source\\repos\\openglsampleapp\\openglsampleapp\\shader\\vertexShader.glsl", "C:\\Users\\ADMIN\\source\\repos\\openglsampleapp\\openglsampleapp\\shader\\FargmentShader.glsl");
	Shader lightShader("C:\\Users\\ADMIN\\source\\repos\\openglsampleapp\\openglsampleapp\\shader\\lvertexShader.glsl", "C:\\Users\\ADMIN\\source\\repos\\openglsampleapp\\openglsampleapp\\shader\\lFargmentShader.glsl");

	World world(shader);

	Sprite3 cube(vertices, shader);
	Sprite3 lighc(vertices, lightShader);

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
	world.addSprite(&lighc);
	






	float x = 0.0f;
	glEnable(GL_DEPTH_TEST);

	cube.updateRotation(40.5f);
	
	int glPositionY = 0;
	float z = 0.0f;

	/*glfwSetCursorPosCallback(window, mouse_callback);*/
	

	// tell GLFW to capture our mouse
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	std::vector<Vertex> verticesQ;
	std::vector<GLuint> indices;
	


	std::pair<std::vector<Vertex>, std::vector<GLuint>> modelData = loadModel("C:\\Users\\ADMIN\\Desktop\\Models\\name.obj");
	GLuint VAO = createMesh(verticesQ,indices);
	
	
	


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
		ImGui::Text("%s",  ("X"+std::to_string(viewVector.x)).c_str());
		ImGui::Text("%s", ("Y" + std::to_string(viewVector.y)).c_str());
		ImGui::Text("%s", ("Z" + std::to_string(viewVector.z)).c_str());


		
		world.lightPos = lighc.position;
		world.viewPos = glm::vec3(lighc.position.x, lighc.position.y + 10.f, lighc.position.z);

		if (ImGui::SliderFloat("x", &x, -1.0f, 40.0f))
		{
			

			lighc.setposition(x,NULL,NULL);
			//world.lightPos = glm::vec3(x, y, 0.f);
			
		};
		if (ImGui::SliderFloat("ydss", &y, -1.0f, 40.0f))
		{
			
			lighc.setposition(NULL,y,NULL);
			//world.lightPos = glm::vec3(x, y, 0.f);

		};
		if (ImGui::SliderFloat("z", &z, -1.0f, 40.0f))
		{

			lighc.setposition(NULL, NULL, z);
			//world.lightPos = glm::vec3(x, y, 0.f);

		};
		if (ImGui::SliderFloat("loadmodel", &laodmodelz, -360.0f, 360.0f))
		{

			
			//world.lightPos = glm::vec3(x, y, 0.f);

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

	
		renderModel(VAO,indices.size(), shader);

		glm::mat4 view = glm::translate(glm::mat4(1.0f), viewVector);
		//view = glm::rotate(view, glm::radians(30.0f), rotatevec);
		//view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

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
//void processInput(GLFWwindow* window)
//{
//	
//	const float cameraSpeed = 0.05f; // adjust accordingly
//	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//		cameraPos += cameraSpeed * cameraFront;
//	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//		cameraPos -= cameraSpeed * cameraFront;
//	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
//	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
//}

void processInput(GLFWwindow* window)
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
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		rotatevecPointer->x += 10.0f;

	}
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
	{
		rotatevecPointer->y += 10.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
	{
		rotatevecPointer->z += 10.0f;
	}
}
;
