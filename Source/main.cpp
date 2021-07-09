/* -[Author:SevenSolaris]------------------------------- */
/* -[GitHub:https://github.com/SevenSolaris]------------ */
/* -[Description:]-------------------------------------- */
// OpenGL "starter" project from following tutorial:
// https://www.youtube.com/watch?v=45MIykWJ-C4
// https://github.com/VictorGordan/opengl-tutorials/tree/main/YoutubeOpenGL%206%20-%20Textures
/* ----------------------------------------------------- */

//╔════════════════════════════════════════════════════════╗
//║ Includes                                               ║
//╚════════════════════════════════════════════════════════╝
#pragma region Includes

#include<iostream>

#include<boost/signals2/signal.hpp>
#include<boost/bind.hpp>

#include<imgui.h>
#include<imgui_impl_glfw.h>
#include<imgui_impl_opengl3.h>

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include "engine/shaderClass.h"
#include "engine/VAO.h"
#include "engine/VBO.h"
#include "engine/EBO.h"
#include "engine/gamewindow.h"
#include "engine/helpers.h"

#include "engine/aabb.h"
#include "engine/ray.h"

#pragma endregion

namespace gui = ImGui;
using namespace std;

/* ----------------------------------------------------- */
/* -[CONSTANTS]----------------------------------------- */
/* ----------------------------------------------------- */
#pragma region Constants

GLfloat vertices[] =
{ //     COORDINATES     /        COLORS      /   TexCoord  //
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
};

// Indices for vertices order
GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 1, 4,
	1, 2, 4,
	2, 3, 4,
	3, 0, 4
};

const int WindowWidth = 800;
const int WindowHeight = 800;

#pragma endregion

/* ---------------------------------------------------------- */
/* -[Testing]------------------------------------------------ */
/* ---------------------------------------------------------- */
#pragma region [Testing]]

#pragma endregion [Testing]

/* ----------------------------------------------------- */
/* -[FUNCTIONS]----------------------------------------- */
/* ----------------------------------------------------- */
#pragma region Functions

void writechar(GameWindow* window, unsigned int codepoint)
{
	std::cout << char(codepoint);
}

void ShowGui();

struct
{
	bool drawTriangles = true;
	float scale = 1.0f;
	bool showStyleEditor = false;
	bool showDemoWindow = false;
	char text[64] = "Test";
} menu;

int main()
{

	vec3 ro(0.0f, 0.0f, 0.0f);
	vec3 rd(1.0f, 0.0f, 0.0f);

	// Initialize glfw
	glfwInit();

	auto workarea = helpers::GetWorkarea();
	recti winrect(0, 0, 1280, 720);
	winrect.layout(workarea, rectlayout::midcenter);
	GameWindow window(winrect.x, winrect.y, winrect.w, winrect.h, "Youtube OpenGL");

	if (!window.Validate())
	{
		std::cout << "Window creation failed. Exiting program." << std::endl;
		glfwTerminate();
		return -1;
	}

	GLFWwindow* winptr = window.GetWindow();

	glfwSetWindowAttrib(winptr, GLFW_MAXIMIZED, 1);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	io.Fonts->AddFontDefault();
	ImFont* font = io.Fonts->AddFontFromFileTTF("fonts\\consola.ttf", 24.0f);
	if (!font)
		cout << "Font was not loaded." << endl;

	io.FontAllowUserScaling = true;
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window.GetWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 330");

	ImGuiStyle* style = &gui::GetStyle();
	//style->ScaleAllSizes(2.0f);

	//gui::SetWindowFontScale(2.0f);

	window.SignalChar.connect(writechar);

	//glViewport(0, 0, 800, 800);

	Shader shader("shaders\\default.vert", "shaders\\default.frag");
	shader.Activate();

	// Declare matrices
	glm::mat4 model(1.0f);
	glm::mat4 view(1.0f);
	glm::mat4 proj(1.0f);

	// Initialize matrices
	view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));
	//proj = glm::perspective(glm::radians(45.0f), (float)(window.Width() / window.Height()), 0.001f, 1000.0f);

	int fwidth, fheight;
	glfwGetFramebufferSize(window.GetWindow(), &fwidth, &fheight);
	proj = glm::perspective(glm::radians(45.0f), float(fwidth) / float(fheight), 0.001f, 1000.0f);

	// Set the matrices in the shader
	int modelLoc = shader.GetUniformLocation("model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	int viewLoc = shader.GetUniformLocation("view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	int projLoc = shader.GetUniformLocation("proj");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

	// Create Vertex array object, vertex buffer object, and element buffer object.
	VAO vao1;
	vao1.Bind();

	VBO vbo1(vertices, sizeof(vertices));
	EBO ebo1(indices, sizeof(indices));

	// Link vertex attributes
	vao1.LinkAttrib(vbo1, 0, 3, GL_FLOAT, 8 * sizeof(float), nullptr);
	vao1.LinkAttrib(vbo1, 1, 3, GL_FLOAT, 8 * sizeof(float),(void*)(3 * sizeof(float)));
	vao1.LinkAttrib(vbo1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	// Unbind
	vao1.Unbind();
	vbo1.Unbind();
	ebo1.Unbind();

	GLuint uniID = shader.GetUniformLocation("scale");

	int widthImg, heightImg, numColCh;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load("Textures\\Caution_Square.png", &widthImg, &heightImg, &numColCh, 0);

	GLuint texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	// glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flagColor);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(bytes);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint tex0Uni = shader.GetUniformLocation("tex0");
	shader.Activate();
	glUniform1i(tex0Uni, 0);

	glEnable(GL_DEPTH_TEST);
	//ImguiIO& io = ImGui::GetIO(); (void)io;


	bool quitFlag = false;
	glm::ivec2 size = window.GetSize();


	//auto style = &gui::GetStyle();
	//style->ScaleAllSizes(2.5f);

	// Simulation Loop
	while (!(window.ShouldClose() || quitFlag)) 
	{
		// Clear the back-buffer
		//glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		auto newSize = window.GetSize();

		//gui::SetWindowFontScale(4.0f);

		shader.Activate();

		if (newSize != size)
		{
			size = newSize;
			int fwidth, fheight;
			glfwGetFramebufferSize(window.GetWindow(), &fwidth, &fheight);
			proj = glm::perspective(glm::radians(45.0f), float(fwidth) / float(fheight), 0.001f, 1000.0f);
			projLoc = shader.GetUniformLocation("proj");
			glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
		}

		glUniform1f(uniID, 1.0f);

		glBindTexture(GL_TEXTURE_2D, texture);
		vao1.Bind();

		if (menu.drawTriangles)
			glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

		//gui::SetWindowFontScale(2.0f);
		//gui::SetNextWindowContentSize(ImVec2((float)window.GetSize().x, (float)window.GetSize().y));

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		const char* label;

		gui::PushFont(font);

		
		if (gui::BeginMainMenuBar())
		{
			if (gui::MenuItem("Exit"))
			{
				quitFlag = true;
			}
			label = window.IsMaximized() ? "Unmaximize" : "Maximize";
			if (gui::MenuItem(label))
			{
				window.SetMaximized(!window.IsMaximized());
			}
			label = window.IsFullscreen() ? "Windowed" : "Fullscreen";
			if (gui::MenuItem(label))
			{
				window.SetFullscreen(!window.IsFullscreen());
			}
			if (gui::BeginMenu("Windows"))
			{
				gui::Checkbox("Style Editor", &menu.showStyleEditor);
				gui::Checkbox("Demo Window", &menu.showDemoWindow);

				gui::EndMenu();
			}
			gui::EndMainMenuBar();
		}

		if (menu.showStyleEditor)
		{
			//gui::Begin("Style Editor");
			gui::ShowStyleEditor();
			//gui::End();
		}

		if (menu.showDemoWindow)
			gui::ShowDemoWindow(&menu.showDemoWindow);

		gui::PopFont();

		gui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		

		window.SwapBuffers();
		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	gui::DestroyContext();

	// Cleanup and terminate
	vao1.Delete();
	vbo1.Delete();
	ebo1.Delete();
	glDeleteTextures(1, &texture);
	shader.Delete();
	return 0;
}

void ShowGui()
{

}

#pragma endregion