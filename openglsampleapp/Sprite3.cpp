#include "Sprite3.hpp"
#define STB_IMAGE_IMPLEMENTATION

Sprite3::Sprite3(const std::vector<float>& vertices, Shader& shader) : shader(shader), modelMatrix(glm::mat4(1.0f)),
rotationAngle(0.0f), rotationAxis(glm::vec3(0.0f, 1.0f, 0.0f))
{
	setupSprite(vertices);


	
}

Sprite3::~Sprite3()
{
}
void Sprite3::setRotation(float angle, const glm::vec3& axis) {
	rotationAngle = angle;
	rotationAxis = axis;
}

void Sprite3::updateRotation(float deltaAngle) {
	rotationAngle += deltaAngle;
}

void Sprite3::setupSprite(const std::vector<float>& vertices) {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void Sprite3::setposition(float x, float y,float z) {
	if (x != NULL) {

	position.x = x;
	}
	if (y != NULL) {

		position.y = y;
	}
	if (z != NULL) {

		position.z = z;
	}




	

}

void Sprite3::render(const glm::mat4& view, const glm::mat4& projection) {

	shader.use();
	//modelMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngle), rotationAxis);
	//modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 2.0f));
	modelMatrix = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 model = glm::mat4(1.0f);
	shader.setMat4("model", modelMatrix);
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);
	shader.setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

}