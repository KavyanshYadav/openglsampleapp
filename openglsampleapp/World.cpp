#include "World.hpp"

World::World(Shader shaderprogram): shader(shaderprogram), lightPos(1.2f, 1.0f, 2.0f), lightColor(1.0f, 1.0f, 1.0f), viewPos(0.0f, 0.0f, 3.0f)
{

}


void World::addSprite(Sprite3* sprite) {
	sprites.push_back(sprite);
}

void World::render(const glm::mat4& view, const glm::mat4& projection) {
	shader.use();
	shader.setVec3("lightPos", lightPos);
	shader.setVec3("lightColor", lightColor);
	shader.setVec3("viewPos", viewPos);



	for (auto sprite : sprites) {
		sprite->shader.use();
		/*sprite->shader.setVec3("lightPos", lightPos);
		sprite->shader.setVec3("lightColor", lightColor);*/
		sprite->render(view, projection);
	}
}


