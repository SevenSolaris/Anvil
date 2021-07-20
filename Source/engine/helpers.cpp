//╔════════════════════════════════════════════════════════╗
//║ Includes                                               ║
//╚════════════════════════════════════════════════════════╝
#pragma region [Includes]

#include "helpers.h"
#include "util.h"

#pragma endregion [Includes]

namespace helpers
{
	glm::ivec2 GetScreenSize()
	{

		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

		return glm::ivec2(mode->width, mode->height);

	}

	recti GetWorkarea()
	{
		recti screen(0, 0, 0, 0);
		glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &screen.x, &screen.y, &screen.w, &screen.h);

		return screen;

	}

}