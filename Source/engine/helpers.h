#ifndef HELPERS_HEADER_FILE
#define HELPERS_HEADER_FILE

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
#endif // HELPERS_HEADER_FILE