#ifndef UTIL_H
#define UTIL_H

/* ---------------------------------------------------------- */
/* -[Helper Macros]------------------------------------------ */
/* ---------------------------------------------------------- */
#pragma region [Helper Macros]

#define CHECKFLAG(value, flag) ((static_cast<unsigned int>(value) & static_cast<unsigned int>(flag)) == static_cast<unsigned int>(flag))
#define ANYFLAG(value, flag) ((static_cast<unsigned int>(value) & static_cast<unsigned int>(flag)) != 0u)

#pragma endregion [Helper Macros]

/* ---------------------------------------------------------- */
/* -[Includes]----------------------------------------------- */
/* ---------------------------------------------------------- */
#pragma region [Includes]

#include<glm/glm.hpp>

#pragma endregion [Includes]

/* ---------------------------------------------------------- */
/* -[Classes]------------------------------------------------ */
/* ---------------------------------------------------------- */
#pragma region [Classes]

class sizei
{
	public:
		int w, h;
		sizei() : w(0), h(0) {}
		sizei(int width, int height) : w(width), h(height) {}
};

class sizef
{
	public:
		float w, h;
		sizef() : w(0.0f), h(0.0f) {}
		sizef(float width, float height) : w(width), h(height) {}
};

enum class rectanchor
{
	// 9 anchor points
	topleft = 1,			// 0
	topcenter = 2,			// 1
	topright = 4,			// 2
	midleft = 8,			// 3
	midcenter = 16,			// 4
	midright = 32,			// 5
	bottomleft = 64,		// 6
	bottomcenter = 128,		// 7
	bottomright = 256,		// 8
};

enum class rectlayout
{
	// 9 anchor points
	topleft = 1,			// 0
	topcenter = 2,			// 1
	topright = 4,			// 2
	midleft = 8,			// 3
	midcenter = 16,			// 4
	midright = 32,			// 5
	bottomleft = 64,		// 6
	bottomcenter = 128,		// 7
	bottomright = 256,		// 8
	// Transformation
	fillwidth = 512,		// 9
	fillheight = 1024,		// 10
	halfwidth = 2048,		// 11
	halfheight = 4096,		// 12
	fill = 1536,			// 13
	halfsize = 6144			// 14
};

class recti
{

public:

	union
	{
		struct {
			int x, y;
		};
		glm::ivec2 pos;
	};
	union
	{
		struct {
			int w, h;
		};
		glm::ivec2 size;
	};

	recti() : x(0), y(0), w(0), h(0) {}
	recti(int x, int y, int width, int height);
	recti(glm::ivec2 pos, glm::ivec2 size);

	glm::ivec2 center();
	glm::ivec2 getanchor(rectanchor anchor);

	void layout(recti parent, rectlayout layout);
	void setanchor(rectanchor anchor, glm::ivec2 pos);

	void inflate(int amount);
	void restructure(int x, int y, int width, int height);

	bool intersects(recti other);
	bool contains(int x, int y);
	bool contains(glm::ivec2 p);

};

class rectf
{
public:

	union {
		struct {
			float x, y;
		};
		glm::vec2 pos;
	};

	union {
		struct {
			float w, h;
		};
		glm::vec2 size;
	};

	rectf() : x(0.0f), y(0.0f), w(0.0f), h(0.0f) {}
	rectf(float x, float y, float width, float height);
	rectf(glm::vec2 pos, glm::vec2 size);

	glm::vec2 center();
	glm::vec2 getanchor(rectanchor anchor);

	void layout(rectf parent, rectlayout layout);
	void setanchor(rectanchor anchor, glm::vec2 pos);

	void inflate(float amount);
	void restructure(float x, float y, float width, float height);

	bool intersects(rectf other);
	bool contains(float x, float y);
};

#pragma endregion [Classes]


// No code beyond this point.
#endif