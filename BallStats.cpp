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
	
	float delta_dist = 0.15f;

	/*timer += deltaTime;
	glm::vec3 m = position - mapVertex;
	speed = glm::vec3(speed.x + a *  m.x, 0.0f, speed.z + a * m.z);
	if (timer >= 0.5) {
		timer = 0;
		printf("%f %f %f\n", speed.x, speed.y, speed.z);
	}
	position = position + speed * deltaTime;
	position.y = radius * 0.7 + map_translate_y + mapGenerator::getMapHeight(position.z, position.x, flatness);*/
	glm::vec3 right = glm::vec3(position.x + delta_dist, 0.0f, position.z);
	right.y = mapGenerator::getMapHeight(right.x, right.z, flatness);
	glm::vec3 left = glm::vec3(position.x - delta_dist, 0.0f, position.z);
	left.y = mapGenerator::getMapHeight(left.x, left.z, flatness);
	glm::vec3 front = glm::vec3(position.x, 0.0f, position.z + delta_dist);
	front.y = mapGenerator::getMapHeight(front.x, front.z, flatness);
	glm::vec3 behind = glm::vec3(position.x, 0.0f, position.z - delta_dist);
	behind.y = mapGenerator::getMapHeight(behind.x, behind.z, flatness);
	float x_diff = right.y - left.y;
	float z_diff = front.y - behind.y;
	speed.x = speed.x - a * x_diff * 0.1f;
	speed.z = speed.z - a * z_diff * 0.1f;
	position = position + deltaTime * speed;
	position.y =map_translate_y + radius * 0.7f + mapGenerator::getMapHeight(position.x, position.z, flatness);
	//printf("x_diff: %f; y_diff: %f\n", x_diff, z_diff);

}

void BallStats::update_positon_ball_collision(float deltaTime, glm::vec3 otherBallVertex, glm::vec3 otherBallDirection) {

}

BallStats::BallStats(glm::vec3 position)
{
	timer = 0.0f;

	radius = 1.0f;
	drop_speed = 1.5f;
	speed = glm::vec3(0.0f, 0.0f, 0.0f);
	a = 0.1f;
	this->position = position;
}

BallStats::~BallStats()
{
}