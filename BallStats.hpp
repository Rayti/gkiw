//
//MK
//


#ifndef BALLSTATS_H
#define BALLSTATS_H
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>



class BallStats
{
public:
	BallStats(glm::vec3 position);
	~BallStats();
	glm::vec3 position;
	glm::vec3 direction;
	float radius;
	float speed_x;
	float speed_y;
	float speed_z;
	float drop_speed;
	float speed;
	float a;
	void update_position(float deltaTime);
	void update_positon_map_touch(float deltaTime, glm::vec3 mapVertex);
	void update_positon_ball_collision(float deltaTime,
		glm::vec3 otherBallVertex,
		glm::vec3 otherBallDirection);

private:

};

#endif // !BALLSTATS_H