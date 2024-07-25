#ifndef WORLD_H
#define WORLD_H

#include <glm/glm.hpp>
#include <vector>
#include "Sprite3.hpp"

class World
{
public:
	
	void addSprite(Sprite3* sprite);
	void render(const glm::mat4& view, const glm::mat4& projection);
	

private:
	std::vector<Sprite3*> sprites;

};
#endif