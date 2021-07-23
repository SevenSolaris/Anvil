#ifndef ENGINE_HEADER_FILE
#define ENGINE_HEADER_FILE

//╔════════════════════════════════════════════════════════╗
//║ Includes                                               ║
//╚════════════════════════════════════════════════════════╝
#pragma region [Includes]

#include<boost/signals2/signal.hpp>
#include<boost/signals2.hpp>
#include "scene.h"
#include "gamewindow.h"
#include <vector>


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
			std::vector<Scene*> sceneList;
		public:
			GameWindow* Window;

			Engine();
			~Engine();

			void SetWindowSize(int width, int height);

			void Run();

			void Terminate();

			int AddScene(Scene* scene);
	};


	/*class Scene;

	bool _initialized = false;

	void init();

	void setWindowSize(int width, int height);

	uint addScene(Scene& scene);

	void terminate();*/

};

#pragma endregion [Engine]

#endif // ENGINE_HEADER_FILE