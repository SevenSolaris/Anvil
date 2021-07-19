//╔════════════════════════════════════════════════════════╗
//║ Includes                                               ║
//╚════════════════════════════════════════════════════════╝
#pragma region [Includes]

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.inl>

#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "gamewindow.h"

#include "types.h"
#include "scene.h"
#include "engine.h"

#pragma endregion [Includes]

//╔════════════════════════════════════════════════════════╗
//║ Engine                                                 ║
//╚════════════════════════════════════════════════════════╝
#pragma region [Engine]

namespace engine
{
	Engine::Engine()
	{
		
		this->Window = new GameWindow(800, 600, "Game Engine");
		if (!this->Window->Validate())
		{
			delete this->Window;
			this->Window = nullptr;
		}

	}

	Engine::~Engine()
	{
		if (this->Window)
		{
			delete this->Window;
			this->Window = nullptr;
		}
	}

	void Engine::SetWindowSize(int width, int height)
	{
		if (this->Window)
		{
			this->Window->Resize(width, height);
			this->Window->CenterScreen();
		}
	}

	void Engine::Terminate()
	{
		if (this->Window)
		{
			this->Window->Destroy();
			delete this->Window;
			this->Window = nullptr;
			abort();
		}
	}

	int Engine::AddScene(Scene* scene)
	{
		if (scene)
		{
			int result = this->sceneList.size();
			this->sceneList.push_back(scene);
			return result;
		}
		return -1;
	}

};

#pragma endregion [Engine]