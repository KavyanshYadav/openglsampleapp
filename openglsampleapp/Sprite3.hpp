#ifndef SPRITE3D_H
#define SPRITE3D_H

#include "Shader.hpp"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Sprite3
{
public:
	
	Sprite3(const std::vector<float>& vertices, Shader& shader);
	~Sprite3();
	void render(const glm::mat4& view, const glm::mat4& projection);
	glm::mat4 modelMatrix;
	void setRotation(float angle, const glm::vec3& axis);
	void updateRotation(float deltaAngle);
	void setposition(float x, float y, float z);
	glm::vec3 position;
	


	Shader& shader;
private:
	GLuint VAO, VBO, texture;
	void setupSprite(const std::vector<float>& vertices);
	float rotationAngle;
	glm::vec3 rotationAxis;


};
#endif