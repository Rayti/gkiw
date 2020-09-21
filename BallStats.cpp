//
//MK
//

#include "BallStats.hpp"
#include "mapGenerator.h"


void BallStats::update_position(float deltaTime) {
	glm::vec3 temp = glm::vec3(0.0f, -drop_speed * deltaTime, 0.0f);
	position = position + temp;
}

void BallStats::update_positon_map_touch(float deltaTime, glm::vec3 mapVertex,
	float flatness, float map_translate_y){

	timer += deltaTime;
	glm::vec3 m = position - mapVertex;
	speed = glm::vec3(speed.x + a *  m.x, 0.0f, speed.z + a * m.z);
	if (timer >= 0.5) {
		timer = 0;
		printf("%f %f %f\n", speed.x, speed.y, speed.z);
	}
	position = position + speed * deltaTime;
	position.y = radius * 0.7 + map_translate_y + mapGenerator::getMapHeight(position.z, position.x, flatness);
}

void BallStats::update_positon_ball_collision(float deltaTime, glm::vec3 otherBallVertex, glm::vec3 otherBallDirection) {

}

BallStats::BallStats(glm::vec3 position)
{
	timer = 0.0f;

	radius = 1.0f;
	drop_speed = 1.5f;
	speed = glm::vec3(0.0f, 0.0f, 0.0f);
	a = 0.01f;
	this->position = position;
}

BallStats::~BallStats()
{
}