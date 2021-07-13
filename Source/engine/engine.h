#pragma once

//╔════════════════════════════════════════════════════════╗
//║ Includes                                               ║
//╚════════════════════════════════════════════════════════╝
#pragma region [Includes]

#include<boost/signals2/signal.hpp>
#include<boost/signals2.hpp>
#include "types.h"
#include "scene.h"
#include "gamewindow.h"


#pragma endregion [Includes]

//╔════════════════════════════════════════════════════════╗
//║ Engine                                                 ║
//╚════════════════════════════════════════════════════════╝
#pragma region [Engine]


namespace engine
{
	class Scene;
	class Engine
	{
		private:
			bool terminateGameLoop = false;
		public:
			GameWindow* Window;

			Engine();
			~Engine();

			void SetWindowSize(int width, int height);

			void Terminate();

			void AddScene(Scene* scene);
	};


	/*class Scene;

	bool _initialized = false;

	void init();

	void setWindowSize(int width, int height);

	uint addScene(Scene& scene);

	void terminate();*/

};

#pragma endregion [Engine]