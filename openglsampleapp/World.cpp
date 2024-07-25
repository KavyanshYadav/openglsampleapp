#include "World.hpp"

void World::addSprite(Sprite3* sprite) {
	sprites.push_back(sprite);
}

void World::render(const glm::mat4& view, const glm::mat4& projection) {
	for (auto sprite : sprites) {
		sprite->shader.use();
		/*sprite->shader.setVec3("lightPos", lightPos);
		sprite->shader.setVec3("lightColor", lightColor);*/
		sprite->render(view, projection);
	}
}


