#pragma once

// https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection

//╔════════════════════════════════════════════════════════╗
//║ Includes                                               ║
//╚════════════════════════════════════════════════════════╝
#pragma region [Includes]

#include<glm/glm.hpp>

#pragma endregion [Includes]

//╔════════════════════════════════════════════════════════╗
//║ Class                                                  ║
//╚════════════════════════════════════════════════════════╝
#pragma region [Class]

using namespace glm;

class aabb
{
public:
	union
	{
		struct
		{
			vec3 min;
			vec3 max;
		};
		vec3 bounds[2];
	};

	aabb(const vec3& min, const vec3& max);

};

#pragma endregion [Class]