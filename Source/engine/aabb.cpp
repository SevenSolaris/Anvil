//╔════════════════════════════════════════════════════════╗
//║ Includes                                               ║
//╚════════════════════════════════════════════════════════╝
#pragma region [Includes]

#include<glm/glm.hpp>

#include "aabb.h"

#pragma endregion [Includes]

using namespace glm;

aabb::aabb(const vec3& min, const vec3& max)
	: min(min), max(max)
{
}

