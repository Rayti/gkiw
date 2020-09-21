//
//MK
//

#include "BallStats.hpp"



void BallStats::update_position(float deltaTime) {
	glm::vec3 temp = glm::vec3(0.0f, -drop_speed * deltaTime, 0.0f);
	position = position + temp;
}

void BallStats::update_positon_map_touch(float deltaTime, glm::vec3 mapVertex){
	glm::vec3 temp_k = position - mapVertex;
	glm::vec3 temp_w = direction;
	glm::vec3 temp_m = temp_k + temp_w;
	temp_m = glm::normalize(temp_m);
	glm::vec3 speed_vertex = a * temp_m + speed * temp_m;
	float cos_alpha = (temp_w.x * speed * temp_k.x +
		temp_w.y * temp_k.y +
		temp_w.z * temp_k.z) / (glm::length(temp_w) * glm::length(temp_k));



	//temp.y = 0.0f;
	//temp = glm::normalize(temp);
	//speed_x += a * temp.x;
	//speed_z += a * temp.z;
	//position = position + temp;
}

void BallStats::update_positon_ball_collision(float deltaTime, glm::vec3 otherBallVertex, glm::vec3 otherBallDirection) {

}

BallStats::BallStats(glm::vec3 position)
{
	radius = 0.008f;
	speed_x = 0.0f;
	speed_y = 0.0f;
	speed_z = 0.0f;
	drop_speed = 1.5f;
	speed = 0.0f;
	a = 0.05f;
	this->position = position;
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
}

BallStats::~BallStats()
{
}