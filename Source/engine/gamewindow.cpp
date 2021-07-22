//╔════════════════════════════════════════════════════════════════════════════════════════════════╗
//║ Includes                                                                                       ║
//╚════════════════════════════════════════════════════════════════════════════════════════════════╝
#pragma region [Includes]

#include<iostream>

#include<imgui.h>
#include<imgui_impl_glfw.h>
#include<imgui_impl_opengl3.h>

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<boost/bind.hpp>
#include<boost/signals2.hpp>
#include "gamewindow.h"
#include "helpers.h"
#include "util.h"

#pragma endregion [Includes]

//╔════════════════════════════════════════════════════════════════════════════════════════════════╗
//║ Class                                                                                          ║
//╚════════════════════════════════════════════════════════════════════════════════════════════════╝
#pragma region [Class]

//╔════════════════════════════════════════════════════════════════════════════════════════════════╗
//║ Static Callbacks                                                                               ║
//╚════════════════════════════════════════════════════════════════════════════════════════════════╝
#pragma region [Static Callbacks]

void GameWindow::StaticCallbackClose(GLFWwindow* window)
{
	void* ptr = glfwGetWindowUserPointer(window);
	if (ptr)
	{
		GameWindow* w = static_cast<GameWindow*>(ptr);
		w->CloseCallback();
	}
}

void GameWindow::StaticCallbackResize(GLFWwindow* window, int width, int height)
{
	void* ptr = glfwGetWindowUserPointer(window);
	if (ptr)
	{
		GameWindow* w = static_cast<GameWindow*>(ptr);
		w->ResizeCallback(width, height);
	}
}

void GameWindow::StaticCallbackReposition(GLFWwindow* window, int x, int y)
{
	void* ptr = glfwGetWindowUserPointer(window);
	if (ptr)
	{
		GameWindow* w = static_cast<GameWindow*>(ptr);
		w->RepositionCallback(x, y);
	}
}

void GameWindow::StaticCallbackFramebufferResize(GLFWwindow* window, int width, int height)
{
	void* ptr = glfwGetWindowUserPointer(window);
	if (ptr)
	{
		GameWindow* w = static_cast<GameWindow*>(ptr);
		w->FramebufferResizeCallback(width, height);
	}
}

void GameWindow::StaticCallbackScale(GLFWwindow* window, float xscale, float yscale)
{
	void* ptr = glfwGetWindowUserPointer(window);
	if (ptr)
	{
		GameWindow* w = static_cast<GameWindow*>(ptr);
		w->ScaleCallback(xscale, yscale);
	}
}

void GameWindow::StaticCallbackMinimized(GLFWwindow* window, int maximized)
{
	void* ptr = glfwGetWindowUserPointer(window);
	if (ptr)
	{
		GameWindow* w = static_cast<GameWindow*>(ptr);
		w->MinimizedCallback(maximized);
	}
}

void GameWindow::StaticCallbackMaximized(GLFWwindow* window, int maximized)
{
	void* ptr = glfwGetWindowUserPointer(window);
	if (ptr)
	{
		GameWindow* w = static_cast<GameWindow*>(ptr);
		w->MaximizedCallback(maximized);
	}
}

void GameWindow::StaticCallbackFocus(GLFWwindow* window, int focused)
{
	void* ptr = glfwGetWindowUserPointer(window);
	if (ptr)
	{
		GameWindow* w = static_cast<GameWindow*>(ptr);
		w->FocusCallback(focused);
	}
}

void GameWindow::StaticCallbackRefresh(GLFWwindow* window)
{
	void* ptr = glfwGetWindowUserPointer(window);
	if (ptr)
	{
		GameWindow* w = static_cast<GameWindow*>(ptr);
		w->RefreshCallback();
	}
}

void GameWindow::StaticCallbackKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	void* ptr = glfwGetWindowUserPointer(window);
	if (ptr)
	{
		GameWindow* w = static_cast<GameWindow*>(ptr);
		w->KeyCallback(key, scancode, action, mods);
	}
}

void GameWindow::StaticCallbackChar(GLFWwindow* window, unsigned int codepoint)
{
	void* ptr = glfwGetWindowUserPointer(window);
	if (ptr)
	{
		GameWindow* w = static_cast<GameWindow*>(ptr);
		w->CharCallback(codepoint);
	}
}

void GameWindow::StaticCallbackMousePos(GLFWwindow* window, double x, double y)
{
	void* ptr = glfwGetWindowUserPointer(window);
	if (ptr)
	{
		GameWindow* w = static_cast<GameWindow*>(ptr);
		w->MousePosCallback(x, y);
	}
}

void GameWindow::StaticCallbackCursorEntered(GLFWwindow* window, int entered)
{
	void* ptr = glfwGetWindowUserPointer(window);
	if (ptr)
	{
		GameWindow* w = static_cast<GameWindow*>(ptr);
		w->CursorEnteredCallback(entered);
	}
}

void GameWindow::StaticCallbackMouseButton(GLFWwindow* window, int button, int action, int mods)
{
	void* ptr = glfwGetWindowUserPointer(window);
	if (ptr)
	{
		GameWindow* w = static_cast<GameWindow*>(ptr);
		w->MousebuttonCallback(button, action, mods);
	}
}

void GameWindow::StaticCallbackScroll(GLFWwindow* window, double xoffset, double yoffset)
{
	void* ptr = glfwGetWindowUserPointer(window);
	if (ptr)
	{
		GameWindow* w = static_cast<GameWindow*>(ptr);
		w->ScrollCallback(xoffset, yoffset);
	}
}

#pragma endregion [Static Callbacks]

//╔════════════════════════════════════════════════════════════════════════════════════════════════╗
//║ Member Callbacks                                                                               ║
//╚════════════════════════════════════════════════════════════════════════════════════════════════╝
#pragma region [Member Callbacks]

// TODO: Utilize boost signals to dispatch events.

void GameWindow::CloseCallback()
{
	this->SignalClose(this);
}

void GameWindow::ResizeCallback(int width, int height)
{
	glViewport(0, 0, width, height);
	this->SignalResize(this, width, height);
}

void GameWindow::RepositionCallback(int x, int y)
{
	this->SignalReposition(this, x, y);
}

void GameWindow::FramebufferResizeCallback(int width, int height)
{
	glViewport(0, 0, width, height);
	this->SignalFramebufferResize(this, width, height);
}

void GameWindow::ScaleCallback(float xscale, float yscale)
{
	this->SignalScale(this, xscale, yscale);
}

void GameWindow::MinimizedCallback(int maximized)
{
	this->SignalMinimized(this, maximized);
}

void GameWindow::MaximizedCallback(int maximized)
{
	this->SignalMaximized(this, maximized);
}

void GameWindow::FocusCallback(int focused)
{
	this->SignalFocus(this, focused);
}

void GameWindow::RefreshCallback()
{
	this->SignalRefresh(this);
}

void GameWindow::KeyCallback(int key, int scancode, int action, int mods)
{
	this->SignalKey(this, key, scancode, action, mods);
}

void GameWindow::CharCallback(unsigned int codepoint)
{
	this->SignalChar(this, codepoint);
}

void GameWindow::MousePosCallback(double x, double y)
{
	this->SignalMousePos(this, x, y);
}

void GameWindow::CursorEnteredCallback(int entered)
{
	this->SignalCursorEntered(this, entered);
}

void GameWindow::MousebuttonCallback(int button, int action, int mods)
{
	this->SignalMouseButton(this, button, action, mods);
}

void GameWindow::ScrollCallback(double xoffset, double yoffset)
{
	this->SignalScroll(this, xoffset, yoffset);
}


#pragma endregion [Member Callbacks]

//╔════════════════════════════════════════════════════════════════════════════════════════════════╗
//║ Constructor/Desctructor                                                                        ║
//╚════════════════════════════════════════════════════════════════════════════════════════════════╝
#pragma region [Constructors/Desctructor]

namespace gui = ImGui;

GameWindow::GameWindow(const char* title, GLFWmonitor* monitor, GLFWwindow* share)
{
	glm::ivec2 size = helpers::GetScreenSize();
	if(monitor)
		this->CreateWindow(0, 0, size.x, size.y, title, monitor, share);
	else
	{
		recti screen = helpers::GetWorkarea();
		recti winrect = recti(0, 0, 800, 600);

		winrect.layout(screen, rectlayout::midcenter);

		this->CreateWindow(winrect.x, winrect.y, winrect.w, winrect.h, title, monitor, share);
	}
}

GameWindow::GameWindow(int x, int y, int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share)
{
	this->CreateWindow(x, y, width, height, title, monitor, share);
}

GameWindow::GameWindow(const recti& bounds, const char* title, GLFWmonitor* monitor, GLFWwindow* share)
	: GameWindow(bounds.x, bounds.y, bounds.w, bounds.h, title, monitor, share)
{
}

GameWindow::GameWindow(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share)
{
	recti screen = helpers::GetWorkarea();
	recti winrect = recti(0, 0, width, height);

	winrect.layout(screen, rectlayout::midcenter);

	this->CreateWindow(winrect.x, winrect.y, winrect.w, winrect.h, title, monitor, share);

}

GameWindow::~GameWindow()
{
	this->Destroy();
}

#pragma endregion [Constructors/Desctructor]

void GameWindow::CreateWindow(int x, int y, int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share)
{
	this->share = share;
	if (title)
		this->title = title;
	else
		this->title = DEFAULT_WINDOW_TITLE;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	// Use CORE profile for modern functions.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_AUTO_ICONIFY, 1);
	//glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	this->window = glfwCreateWindow(width, height, title, monitor, share);
	if (this->window)
	{
		this->oldRect = recti(x, y, width, height);

		glfwMakeContextCurrent(this->window);
		glfwSetWindowUserPointer(this->window, this);
		glfwSetWindowPos(this->window, x, y);


		gladLoadGL();

		glViewport(0, 0, width, height);

		// Hook up callbacks.
		glfwSetWindowCloseCallback(this->window,			&GameWindow::StaticCallbackClose);
		glfwSetWindowSizeCallback(this->window,				&GameWindow::StaticCallbackResize);
		glfwSetWindowRefreshCallback(this->window,			&GameWindow::StaticCallbackRefresh);
		glfwSetWindowPosCallback(this->window,				&GameWindow::StaticCallbackReposition);
		glfwSetFramebufferSizeCallback(this->window,		&GameWindow::StaticCallbackFramebufferResize);
		glfwSetWindowContentScaleCallback(this->window,		&GameWindow::StaticCallbackScale);
		glfwSetWindowIconifyCallback(this->window,			&GameWindow::StaticCallbackMinimized);
		glfwSetWindowMaximizeCallback(this->window,			&GameWindow::StaticCallbackMaximized);
		glfwSetWindowFocusCallback(this->window,			&GameWindow::StaticCallbackFocus);
		glfwSetWindowRefreshCallback(this->window,			&GameWindow::StaticCallbackRefresh);
		glfwSetKeyCallback(this->window,					&GameWindow::StaticCallbackKey);
		glfwSetCharCallback(this->window,					&GameWindow::StaticCallbackChar);
		glfwSetCursorPosCallback(this->window,				&GameWindow::StaticCallbackMousePos);
		glfwSetCursorEnterCallback(this->window,			&GameWindow::StaticCallbackCursorEntered);
		glfwSetMouseButtonCallback(this->window,			&GameWindow::StaticCallbackMouseButton);
		glfwSetScrollCallback(this->window,					&GameWindow::StaticCallbackScroll);


	}
}

bool GameWindow::Validate()
{
	return this->window != nullptr;
}

//╔════════════════════════════════════════════════════════════════════════════════════════════════╗
//║ Getters                                                                                        ║#pragma region [Getters]
//╚════════════════════════════════════════════════════════════════════════════════════════════════╝
GLFWwindow* GameWindow::GetWindow()
{
	return this->window;
}

GLFWmonitor* GameWindow::GetMonitor()
{
	if (!this->Validate())
		return nullptr;
	return glfwGetWindowMonitor(this->window);
}

const char* GameWindow::GetTitle()
{
	return this->title;
}

recti GameWindow::GetRect()
{
	recti result(0, 0, 0, 0);
	this->GetRect(&result.x, &result.y, &result.w, &result.h);
	return result;
}

void GameWindow::GetRect(int* x, int* y, int* width, int* height)
{
	if (this->Validate())
	{
		glfwGetWindowPos(this->window, x, y);
		glfwGetWindowSize(this->window, width, height);
	}
}

glm::ivec2 GameWindow::GetPos()
{
	glm::ivec2 result(0, 0);
	if (this->Validate())
		glfwGetWindowPos(this->window, &result.x, &result.y);
	return result;
}

void GameWindow::GetPos(int* x, int* y)
{
	if (this->Validate())
		glfwGetWindowPos(this->window, x, y);
}

glm::ivec2 GameWindow::GetSize()
{
	glm::ivec2 result(0, 0);
	if (this->Validate())
		glfwGetWindowSize(this->window, &result.x, &result.y);
	return result;
}

void GameWindow::GetSize(int* width, int* height)
{
	if (this->Validate())
		glfwGetWindowSize(this->window, width, height);
}

int GameWindow::Width()
{
	int w, h;
	glfwGetWindowSize(this->window, &w, &h);
	return w;
}

int GameWindow::Height()
{
	int w, h;
	glfwGetWindowSize(this->window, &w, &h);
	return h;
}

bool GameWindow::IsFullscreen()
{
	return this->GetMonitor();
}

bool GameWindow::IsMaximized()
{
	if (!this->Validate())
		return false;
	return glfwGetWindowAttrib(this->window, GLFW_MAXIMIZED);
}

bool GameWindow::IsIconified()
{
	if (!this->Validate())
		return false;
	return glfwGetWindowAttrib(this->window, GLFW_ICONIFIED);
}

#pragma endregion [Getters]

//╔════════════════════════════════════════════════════════════════════════════════════════════════╗
//║ Setters                                                                                        ║#pragma region [Setters]
//╚════════════════════════════════════════════════════════════════════════════════════════════════╝
void GameWindow::SetTitle(const char* title)
{
	if (this->Validate())
	{
		glfwSetWindowTitle(this->window, title);
		this->title = title;
	}
}

void GameWindow::SetMonitor(GLFWmonitor* monitor, int refreshRate)
{
	if (!this->Validate())
		return;
	glm::ivec2 size = helpers::GetScreenSize();
	if (monitor)
	{
		this->SetMonitor(monitor, 0, 0, size.x, size.y, refreshRate);
	}
	else
	{
		this->SetMonitor(monitor, this->oldRect.x, this->oldRect.y, this->oldRect.w, this->oldRect.h, refreshRate);
	}
}

void GameWindow::SetMonitor(GLFWmonitor* monitor, int x, int y, int width, int height, int refreshRate)
{
	if (!this->Validate())
		return;
	if (monitor)
	{
		this->GetRect(&this->oldRect.x, &this->oldRect.y, &this->oldRect.w, &this->oldRect.h);
	}
	glfwSetWindowMonitor(this->window, monitor, x, y, width, height, refreshRate);
}

void GameWindow::CenterScreen()
{
	if (!this->Validate())
		return;

	recti screen = helpers::GetWorkarea();
	recti winrect = this->GetRect();

	winrect.layout(screen, rectlayout::midcenter);

	this->SetRect(winrect);
}

void GameWindow::SetRect(recti rect)
{
	this->SetPos(rect.x, rect.y);
	this->Resize(rect.w, rect.h);
}

void GameWindow::SetRect(int x, int y, int width, int height)
{
	this->SetPos(x, y);
	this->Resize(width, height);
}

void GameWindow::SetPos(glm::ivec2 pos)
{
	this->SetPos(pos.x, pos.y);
}

void GameWindow::SetPos(int x, int y)
{
	if (!this->Validate())
		return;
	glfwSetWindowPos(this->window, x, y);
}

void GameWindow::Resize(glm::ivec2 size)
{
	this->Resize(size.x, size.y);
}

void GameWindow::Resize(int width, int height)
{
	if (!this->Validate())
		return;
	glfwSetWindowSize(this->window, width, height);
}

void GameWindow::SetFullscreen(bool v)
{
	if (!this->Validate())
		return;
	if (v && !this->GetMonitor())
	{
		this->SetMonitor(glfwGetPrimaryMonitor());
	}
	else
	{
		this->SetMonitor(nullptr);
	}
}

void GameWindow::SetMaximized(bool v)
{
	if (!this->Validate())
		return;
	if (v)
		glfwMaximizeWindow(this->window);
	else
		glfwRestoreWindow(this->window);
}

void GameWindow::SetIconified(bool v)
{
	if (!this->Validate())
		return;
	if (v)
		glfwIconifyWindow(this->window);
	else
		glfwRestoreWindow(this->window);
}

void GameWindow::Maximize()
{
	if (!this->Validate())
		return;
	glfwMaximizeWindow(this->window);
}

void GameWindow::Iconify()
{
	if (!this->Validate())
		return;
	glfwIconifyWindow(this->window);
}

void GameWindow::Restore()
{
	if (!this->Validate())
		return;
	glfwRestoreWindow(this->window);
}

#pragma endregion [Setters]

bool GameWindow::ShouldClose()
{
	return !this->Validate() || glfwWindowShouldClose(this->window);
}

void GameWindow::SwapBuffers()
{
	if (!this->Validate())
		return;
	glfwSwapBuffers(this->window);
}

void GameWindow::Destroy()
{
	if (this->Validate())
		glfwDestroyWindow(this->window);
	this->window = nullptr;
	this->title = nullptr;
	this->share = nullptr;

	// Close signals
	SignalClose.disconnect_all_slots();
	SignalResize.disconnect_all_slots();
	SignalReposition.disconnect_all_slots();
	SignalFramebufferResize.disconnect_all_slots();
	SignalScale.disconnect_all_slots();
	SignalMinimized.disconnect_all_slots();
	SignalMaximized.disconnect_all_slots();
	SignalFocus.disconnect_all_slots();
	SignalRefresh.disconnect_all_slots();
	SignalKey.disconnect_all_slots();
	SignalChar.disconnect_all_slots();
	SignalMousePos.disconnect_all_slots();
	SignalCursorEntered.disconnect_all_slots();
	SignalMouseButton.disconnect_all_slots();
	SignalScroll.disconnect_all_slots();

}

#pragma endregion [Class]