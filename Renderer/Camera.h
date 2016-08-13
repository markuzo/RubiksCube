/*********************************************************************************/
/* Title:		Camera.h   																									         */
/* Author:	Markus Schlafli																							         */
/* Date:		11-Aug-2013																								           */
/*********************************************************************************/

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

struct Camera {

	glm::vec3 pos;
	glm::vec3 dir;
	float speed;

	Camera() :
			pos( glm::vec3( 0.0f, 0.0f, 0.0f ) ),
			dir( glm::vec3( 0.0f, 0.0f, -1.0f ) ),
			speed( 0.005f ) {}
};

#endif // CAMERA_H