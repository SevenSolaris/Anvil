// https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection

//╔════════════════════════════════════════════════════════╗
//║ Includes                                               ║
//╚════════════════════════════════════════════════════════╝
#pragma region [Includes]

#include<glm/glm.hpp>

#include "ray.h"

#pragma endregion [Includes]

ray::ray(const vec3& origin, const vec3& direction)
	:origin(origin), direction(direction)
{
	this->invdir = 1.0f / direction;
	this->sign[0] = (this->invdir.x < 0);
	this->sign[1] = (this->invdir.y < 0);
	this->sign[2] = (this->invdir.z < 0);
}

// TODO: I'm not sure if this actually works, I yoinked it from the internet.
// I may end up deciding to rewrite it with my own code.
// After all, it's just trigonometry. y = mx+b, yadda yadda.
bool ray::intersects(const aabb& box)
{
	float txmin, txmax, tymin, tymax, tzmin, tzmax;

	txmin = (box.bounds[this->sign[0]].x - this->origin.x) * this->invdir.x;
	txmax = (box.bounds[1 - this->sign[0]].x - this->origin.x) * this->invdir.x;
	tymin = (box.bounds[this->sign[1]].y - this->origin.y) * this->invdir.y;
	tymax = (box.bounds[1 - this->sign[1]].y - this->origin.y) * this->invdir.y;

	if ((txmin > tymax) || (tymin > txmax))
		return false;
	if (tymin > txmin)
		txmin = tymin;
	if (tymax < txmax)
		txmax = tymax;

	tzmin = (box.bounds[this->sign[2]].z - this->origin.z) * this->invdir.z;
	tzmax = (box.bounds[1 - this->sign[2]].z - this->origin.z) * this->invdir.z;

	if ((txmin > tzmax) || (tzmin > txmax))
		return false;

	return true;

}