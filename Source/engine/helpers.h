#ifndef GAME_HELPERS_H
#define GAME_HELPERS_H

/* ---------------------------------------------------------- */
/* -[Includes]----------------------------------------------- */
/* ---------------------------------------------------------- */
#pragma region [Includes]

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>

#include "util.h"

#pragma endregion [Includes]

/* ---------------------------------------------------------- */
/* -[Helpers]------------------------------------------------ */
/* ---------------------------------------------------------- */
#pragma region [Helpers]

namespace helpers
{
	glm::ivec2 GetScreenSize();
	recti GetWorkarea();
	
}

#pragma endregion [Helpers]

// No code beyond this point.
#endif