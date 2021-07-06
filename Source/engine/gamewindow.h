#pragma once

//╔════════════════════════════════════════════════════════════════════════════════════════════════╗
//║ Includes                                                                                       ║
//╚════════════════════════════════════════════════════════════════════════════════════════════════╝
#pragma region [Includes]

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<boost/signals2.hpp>

#include "util.h"
#include "strings.h"

#pragma endregion [Includes]

//╔════════════════════════════════════════════════════════════════════════════════════════════════╗
//║ Defines                                                                                        ║
//╚════════════════════════════════════════════════════════════════════════════════════════════════╝
#pragma region [Defines]

#ifndef DEFAULT_WINDOW_WIDTH
#define DEFAULT_WINDOW_WIDTH 1280
#endif

#ifndef DEFAULT_WINDOW_HEIGHT 720
#define DEFAULT_WINDOW_HEIGHT
#endif

#pragma endregion [Defines]

//╔════════════════════════════════════════════════════════════════════════════════════════════════╗
//║ Class                                                                                          ║
//╚════════════════════════════════════════════════════════════════════════════════════════════════╝
#pragma region [Class]

using namespace boost::signals2;

class GameWindow
{
	public:

		//╔════════════════════════════════════════════════════════════════════════════════════════════════╗
		//║ Fields                                                                                         ║
		//╚════════════════════════════════════════════════════════════════════════════════════════════════╝
		#pragma region [Fields]

		recti oldRect = recti(0, 0, 0, 0);
		GLFWwindow* window = nullptr;
		const char* title = nullptr;
		GLFWwindow* share = nullptr;

		#pragma region [Signals]

		boost::signals2::signal<void(GameWindow*)>                     SignalClose				= boost::signals2::signal<void(GameWindow*)>();
		boost::signals2::signal<void(GameWindow*, int, int)>           SignalResize				= boost::signals2::signal<void(GameWindow*, int, int)>();
		boost::signals2::signal<void(GameWindow*, int, int)>           SignalReposition			= boost::signals2::signal<void(GameWindow*, int, int)>();
		boost::signals2::signal<void(GameWindow*, int, int)>           SignalFramebufferResize	= boost::signals2::signal<void(GameWindow*, int, int)>();
		boost::signals2::signal<void(GameWindow*, float, float)>       SignalScale				= boost::signals2::signal<void(GameWindow*, float, float)>();
		boost::signals2::signal<void(GameWindow*, int)>                SignalMinimized			= boost::signals2::signal<void(GameWindow*, int)>();
		boost::signals2::signal<void(GameWindow*, int)>                SignalMaximized			= boost::signals2::signal<void(GameWindow*, int)>();
		boost::signals2::signal<void(GameWindow*, int)>                SignalFocus				= boost::signals2::signal<void(GameWindow*, int)>();
		boost::signals2::signal<void(GameWindow*)>                     SignalRefresh			= boost::signals2::signal<void(GameWindow*)>();
		boost::signals2::signal<void(GameWindow*, int, int, int, int)> SignalKey				= boost::signals2::signal<void(GameWindow*, int, int, int, int)>();
		boost::signals2::signal<void(GameWindow*, unsigned int)>       SignalChar				= boost::signals2::signal<void(GameWindow*, unsigned int)>();
		boost::signals2::signal<void(GameWindow*, double, double)>     SignalMousePos			= boost::signals2::signal<void(GameWindow*, double, double)>();
		boost::signals2::signal<void(GameWindow*, int)>                SignalCursorEntered		= boost::signals2::signal<void(GameWindow*, int)>();
		boost::signals2::signal<void(GameWindow*, int, int, int)>      SignalMouseButton		= boost::signals2::signal<void(GameWindow*, int, int, int)>();
		boost::signals2::signal<void(GameWindow*, double, double)>     SignalScroll				= boost::signals2::signal<void(GameWindow*, double, double)>();

		#pragma endregion [Signals]

		//╔════════════════════════════════════════════════════════════════════════════════════════════════╗
		//║ Constructor/Desctructor                                                                        ║
		//╚════════════════════════════════════════════════════════════════════════════════════════════════╝
		#pragma region [Constructors/Desctructor]

		GameWindow(const char* title = DEFAULT_WINDOW_TITLE, GLFWmonitor* monitor = nullptr, GLFWwindow* share = nullptr);
		GameWindow(const recti& bounds, const char* title = DEFAULT_WINDOW_TITLE, GLFWmonitor* monitor = nullptr, GLFWwindow* share = nullptr);
		GameWindow(int x, int y, int width, int height, const char* title = DEFAULT_WINDOW_TITLE, GLFWmonitor* monitor = nullptr, GLFWwindow* share = nullptr);
		GameWindow(int width, int height, const char* title = DEFAULT_WINDOW_TITLE, GLFWmonitor* monitor = nullptr, GLFWwindow* share = nullptr);
		~GameWindow();

		#pragma endregion [Constructors/Desctructor]

		/// <summary>
		/// Validate that the window was created successfully.
		/// </summary>
		bool Validate();

		//╔════════════════════════════════════════════════════════════════════════════════════════════════╗
		//║ Getters                                                                                        ║
		//╚════════════════════════════════════════════════════════════════════════════════════════════════╝
		#pragma region [Getters]

		GLFWwindow* GetWindow();

		const char* GetTitle();
		GLFWmonitor* GetMonitor();

		recti GetRect();
		void GetRect(int* x, int* y, int* width, int* height);

		glm::ivec2 GetPos();
		void GetPos(int* x, int* y);

		glm::ivec2 GetSize();
		void GetSize(int* width, int* height);

		int Width();
		int Height();

		bool IsFullscreen();
		bool IsMaximized();
		bool IsIconified();


		#pragma endregion [Getters]

		//╔════════════════════════════════════════════════════════════════════════════════════════════════╗
		//║ Setters                                                                                        ║
		//╚════════════════════════════════════════════════════════════════════════════════════════════════╝
		#pragma region [Setters]

		void SetTitle(const char* title);

		void SetMonitor(GLFWmonitor* monitor, int refreshRate = GLFW_DONT_CARE);
		void SetMonitor(GLFWmonitor* monitor, int x, int y, int width, int height, int refreshRate = GLFW_DONT_CARE);

		void CenterScreen();
		void SetRect(recti rect);
		void SetRect(int x, int y, int width, int height);

		void SetPos(glm::ivec2 pos);
		void SetPos(int x, int y);

		void Resize(glm::ivec2 size);
		void Resize(int width, int height);

		void SetFullscreen(bool v = true);
		void SetMaximized(bool v = true);
		void SetIconified(bool v = true);

		void Maximize();
		void Iconify();
		void Restore();

		#pragma endregion [Setters]

		/// <summary>
		/// Check if the window should close (hinting that it is time to end your game loop).
		/// </summary>
		bool ShouldClose();

		void SwapBuffers();

		void Destroy();
	private:

		void CreateWindow(int x, int y, int width, int height, const char* title = DEFAULT_WINDOW_TITLE, GLFWmonitor* monitor = glfwGetPrimaryMonitor(), GLFWwindow* window = nullptr);

		#pragma endregion [Fields]

		//╔════════════════════════════════════════════════════════════════════════════════════════════════╗
		//║ Static Callbacks                                                                               ║
		//╚════════════════════════════════════════════════════════════════════════════════════════════════╝
		#pragma region [Static Callbacks]

		static void StaticCallbackClose(GLFWwindow* window);
		static void StaticCallbackResize(GLFWwindow* window, int width, int height);
		static void StaticCallbackReposition(GLFWwindow* window, int x, int y);
		static void StaticCallbackFramebufferResize(GLFWwindow* window, int width, int height);
		static void StaticCallbackScale(GLFWwindow* window, float xscale, float yscale);
		static void StaticCallbackMinimized(GLFWwindow* window, int maximized);
		static void StaticCallbackMaximized(GLFWwindow* window, int maximized);
		static void StaticCallbackFocus(GLFWwindow* window, int focused);
		static void StaticCallbackRefresh(GLFWwindow* window);
		static void StaticCallbackKey(GLFWwindow* w, int key, int scancode, int action, int mods);
		static void StaticCallbackChar(GLFWwindow* window, unsigned int codepoint);
		static void StaticCallbackMousePos(GLFWwindow* window, double x, double y);
		static void StaticCallbackCursorEntered(GLFWwindow* window, int entered);
		static void StaticCallbackMouseButton(GLFWwindow* window, int button, int action, int mods);
		static void StaticCallbackScroll(GLFWwindow* window, double xoffset, double yoffset);

		#pragma endregion [Static Callbacks]

		//╔════════════════════════════════════════════════════════════════════════════════════════════════╗
		//║ Member Callbacks                                                                               ║
		//╚════════════════════════════════════════════════════════════════════════════════════════════════╝
		#pragma region [Member Callbacks]

		void CloseCallback();
		void ResizeCallback(int width, int height);
		void RepositionCallback(int x, int y);
		void FramebufferResizeCallback(int width, int height);
		void ScaleCallback(float xscale, float yscale);
		void MinimizedCallback(int maximized);
		void MaximizedCallback(int maximized);
		void FocusCallback(int focused);
		void RefreshCallback();
		void KeyCallback(int key, int scancode, int action, int mods);
		void CharCallback(unsigned int codepoint);
		void MousePosCallback(double x, double y);
		void CursorEnteredCallback(int entered);
		void MousebuttonCallback(int button, int action, int mods);
		void ScrollCallback(double xoffset, double yoffset);


		#pragma endregion [Member Callbacks]

		//╔════════════════════════════════════════════════════════════════════════════════════════════════╗
		//║ Operators                                                                                      ║
		//╚════════════════════════════════════════════════════════════════════════════════════════════════╝
		#pragma region [Operators]


		#pragma endregion [Operators]

};

#pragma endregion [Class]