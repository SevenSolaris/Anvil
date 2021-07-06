#pragma once

//╔════════════════════════════════════════════════════════╗
//║ Includes                                               ║
//╚════════════════════════════════════════════════════════╝
#pragma region [Includes]

#include<glm/glm.hpp>

#include "aabb.h"

#pragma endregion [Includes]

//╔════════════════════════════════════════════════════════╗
//║ Class                                                  ║
//╚════════════════════════════════════════════════════════╝
#pragma region [Class]

using namespace glm;

class ray
{
public:
	vec3 origin;
	vec3 direction;
	vec3 invdir;
	int sign[3];

	ray(const vec3& origin, const vec3& direction);

	bool intersects(const aabb& bounds);


};

#pragma endregion [Class]