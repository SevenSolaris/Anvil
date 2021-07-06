/* ---------------------------------------------------------- */
/* -[Includes]----------------------------------------------- */
/* ---------------------------------------------------------- */
#pragma region [Includes]

#include<glm/glm.hpp>
#include "util.h"

#pragma endregion [Includes]

/* ---------------------------------------------------------- */
/* -[Rectangle]---------------------------------------------- */
/* ---------------------------------------------------------- */
#pragma region [Rectangle]

/* ---------------------------------------------------------- */
/* -[int]---------------------------------------------------- */
/* ---------------------------------------------------------- */
#pragma region [int]

recti::recti(int x, int y, int width, int height)
{
	this->x = x;
	this->y = y;
	this->w = width;
	this->h = height;
}

recti::recti(glm::ivec2 pos, glm::ivec2 size)
{
	this->pos = pos;
	this->size = size;
}

glm::ivec2 recti::center()
{
	return glm::ivec2(this->x + this->w / 2, this->y + this->h / 2);
}

void recti::layout(recti parent, rectlayout layout)
{
	// Check transformation flags first
	if (ANYFLAG(layout, 7680))
	{
		// Check fill
		if (ANYFLAG(layout, rectlayout::fill))
		{
			// Fill width
			if (CHECKFLAG(layout, rectlayout::fillwidth))
			{
				this->x = parent.x;
				this->w = parent.w;
			}
			if (CHECKFLAG(layout, rectlayout::fillheight))
			{
				this->y = parent.y;
				this->h = parent.h;
			}
		}
		// Half width
		else if (ANYFLAG(layout, rectlayout::halfsize))
		{
			if (CHECKFLAG(layout, rectlayout::halfwidth))
			{
				this->w = parent.w / 2;
			}
			if (CHECKFLAG(layout, rectlayout::halfheight))
			{
				this->h = parent.h / 2;
			}
		}
	}

	// Check anchors in layout
	if (ANYFLAG(layout, 511))
	{
		// Check top
		if (ANYFLAG(layout, 7))
		{
			if (CHECKFLAG(layout, rectlayout::topleft))
			{
				this->setanchor(rectanchor::topleft, parent.getanchor(rectanchor::topleft));
			}
			else if (CHECKFLAG(layout, rectlayout::topcenter))
			{
				this->setanchor(rectanchor::topcenter, parent.getanchor(rectanchor::topcenter));
			}
			else if (CHECKFLAG(layout, rectlayout::topright))
			{
				this->setanchor(rectanchor::topright, parent.getanchor(rectanchor::topright));
			}
		}
		// Middle
		else if (ANYFLAG(layout, 56))
		{
			if (CHECKFLAG(layout, rectlayout::midleft))
			{
				this->setanchor(rectanchor::midleft, parent.getanchor(rectanchor::midleft));
			}
			else if (CHECKFLAG(layout, rectlayout::midcenter))
			{
				this->setanchor(rectanchor::midcenter, parent.getanchor(rectanchor::midcenter));
			}
			else if (CHECKFLAG(layout, rectlayout::midright))
			{
				this->setanchor(rectanchor::midright, parent.getanchor(rectanchor::midright));
			}
		}
		// Bottom
		else if (ANYFLAG(layout, 448))
		{
			if (CHECKFLAG(layout, rectlayout::bottomleft))
			{
				this->setanchor(rectanchor::bottomleft, parent.getanchor(rectanchor::bottomleft));
			}
			else if (CHECKFLAG(layout, rectlayout::bottomcenter))
			{
				this->setanchor(rectanchor::bottomcenter, parent.getanchor(rectanchor::bottomcenter));
			}
			else if (CHECKFLAG(layout, rectlayout::bottomright))
			{
				this->setanchor(rectanchor::bottomright, parent.getanchor(rectanchor::bottomright));
			}
		}
	}

}

glm::ivec2 recti::getanchor(rectanchor anchor)
{
	if (ANYFLAG(anchor, 511))
	{
		// Check top
		if (ANYFLAG(anchor, 7))
		{
			if (CHECKFLAG(anchor, rectlayout::topleft))
			{
				return glm::ivec2(this->x, this->y);
			}
			else if (CHECKFLAG(anchor, rectlayout::topcenter))
			{
				return glm::ivec2(this->x + (this->w / 2), this->y);
			}
			else if (CHECKFLAG(anchor, rectlayout::topright))
			{
				return glm::ivec2(this->x + this->w, this->y);
			}
		}
		// Middle
		else if (ANYFLAG(anchor, 56))
		{
			if (CHECKFLAG(anchor, rectlayout::midleft))
			{
				return glm::ivec2(this->x, this->y + (this->h / 2));
			}
			else if (CHECKFLAG(anchor, rectlayout::midcenter))
			{
				return glm::ivec2(this->x + (this->w / 2), this->y + (this->h / 2));
			}
			else if (CHECKFLAG(anchor, rectlayout::midright))
			{
				return glm::ivec2(this->x + this->w, this->y + (this->h / 2));
			}
		}
		// Bottom
		else if (ANYFLAG(anchor, 448))
		{
			if (CHECKFLAG(anchor, rectlayout::bottomleft))
			{
				return glm::ivec2(this->x, this->y + this->h);
			}
			else if (CHECKFLAG(anchor, rectlayout::bottomcenter))
			{
				return glm::ivec2(this->x + (this->w / 2), this->y + this->h);
			}
			else if (CHECKFLAG(anchor, rectlayout::bottomright))
			{
				return glm::ivec2(this->x + this->w, this->y + this->h);
			}
		}
	}
	return glm::ivec2(this->x, this->y);
}

void recti::setanchor(rectanchor anchor, glm::ivec2 pos)
{
	if (ANYFLAG(anchor, 511))
	{
		// Check top
		if (ANYFLAG(anchor, 7))
		{
			if (CHECKFLAG(anchor, rectlayout::topleft))
			{
				//return glm::ivec2(this->x, this->y);
				this->x = pos.x;
				this->y = pos.y;
			}
			else if (CHECKFLAG(anchor, rectlayout::topcenter))
			{
				this->x = pos.x - (this->w / 2);
				this->y = pos.y;
				//return glm::ivec2(this->x + (this->w / 2), this->y);
			}
			else if (CHECKFLAG(anchor, rectlayout::topright))
			{
				this->x = pos.x - this->w;
				this->y = pos.y;
				//return glm::ivec2(this->x + this->w, this->y);
			}
		}
		// Middle
		else if (ANYFLAG(anchor, 56))
		{
			if (CHECKFLAG(anchor, rectlayout::midleft))
			{
				this->x = pos.x;
				this->y = pos.y - (this->h / 2);
				//return glm::ivec2(this->x, this->y + (this->h / 2));
			}
			else if (CHECKFLAG(anchor, rectlayout::midcenter))
			{
				this->x = pos.x - (this->w / 2);
				this->y = pos.y - (this->h / 2);
				//return glm::ivec2(this->x + (this->w / 2), this->y + (this->h / 2));
			}
			else if (CHECKFLAG(anchor, rectlayout::midright))
			{
				this->x = pos.x - this->w;
				this->y = pos.y - (this->h / 2);
				//return glm::ivec2(this->x + this->w, this->y + (this->h / 2));
			}
		}
		// Bottom
		else if (ANYFLAG(anchor, 448))
		{
			if (CHECKFLAG(anchor, rectlayout::bottomleft))
			{
				this->x = pos.x;
				this->y = pos.y - this->h;
			}
			else if (CHECKFLAG(anchor, rectlayout::bottomcenter))
			{
				this->x = pos.x - (this->w / 2);
				this->y = pos.y - this->h;
			}
			else if (CHECKFLAG(anchor, rectlayout::bottomright))
			{
				this->x = pos.x - this->w;
				this->y = pos.y - this->h;
			}
		}
	}
}

void recti::inflate(int amount)
{
	this->x -= amount;
	this->y -= amount;
	this->w += (amount * 2);
	this->h += (amount * 2);
}

bool recti::intersects(recti other)
{
	return
		this->x < (other.x + other.w) &&
		other.x < (this->x + this->w) &&
		this->y < (other.y + other.h) &&
		other.y < (this->y + this->h);
}

bool recti::contains(int x, int y)
{
	if (!(this->w | this->h))
		return false;
	return
		x >= this->x &&
		x < (this->x + this->w) &&
		y >= this->y &&
		y < (this->y + this->h);
}

bool recti::contains(glm::ivec2 p)
{
	return this->contains(p.x, p.y);
}

void recti::restructure(int x, int y, int width, int height)
{
	this->x = x;
	this->y = y;
	this->w = width;
	this->h = height;
}

#pragma endregion [int]

/* ---------------------------------------------------------- */
/* -[float]-------------------------------------------------- */
/* ---------------------------------------------------------- */
#pragma region [float]

rectf::rectf(float x, float y, float width, float height)
{
	this->restructure(x, y, width, height);
}

rectf::rectf(glm::vec2 pos, glm::vec2 size)
{
	this->pos = pos;
	this->size = size;
}

glm::vec2 rectf::center()
{
	return glm::vec2(this->x + this->w / 2.0f, this->y + this->h / 2.0f);
}

glm::vec2 rectf::getanchor(rectanchor anchor)
{
	if (ANYFLAG(anchor, 511))
	{
		// Check top
		if (ANYFLAG(anchor, 7))
		{
			if (CHECKFLAG(anchor, rectlayout::topleft))
			{
				return glm::vec2(this->x, this->y);
			}
			else if (CHECKFLAG(anchor, rectlayout::topcenter))
			{
				return glm::vec2(this->x + (this->w / 2.0f), this->y);
			}
			else if (CHECKFLAG(anchor, rectlayout::topright))
			{
				return glm::vec2(this->x + this->w, this->y);
			}
		}
		// Middle
		else if (ANYFLAG(anchor, 56))
		{
			if (CHECKFLAG(anchor, rectlayout::midleft))
			{
				return glm::vec2(this->x, this->y + (this->h / 2.0f));
			}
			else if (CHECKFLAG(anchor, rectlayout::midcenter))
			{
				return glm::vec2(this->x + (this->w / 2.0f), this->y + (this->h / 2.0f));
			}
			else if (CHECKFLAG(anchor, rectlayout::midright))
			{
				return glm::vec2(this->x + this->w, this->y + (this->h / 2.0f));
			}
		}
		// Bottom
		else if (ANYFLAG(anchor, 448))
		{
			if (CHECKFLAG(anchor, rectlayout::bottomleft))
			{
				return glm::vec2(this->x, this->y + this->h);
			}
			else if (CHECKFLAG(anchor, rectlayout::bottomcenter))
			{
				return glm::vec2(this->x + (this->w / 2.0f), this->y + this->h);
			}
			else if (CHECKFLAG(anchor, rectlayout::bottomright))
			{
				return glm::vec2(this->x + this->w, this->y + this->h);
			}
		}
	}
	return glm::vec2(this->x, this->y);
}

void rectf::layout(rectf parent, rectlayout layout)
{
	// Check transformation flags first
	if (ANYFLAG(layout, 7680))
	{
		// Check fill
		if (ANYFLAG(layout, rectlayout::fill))
		{
			// Fill width
			if (CHECKFLAG(layout, rectlayout::fillwidth))
			{
				this->x = parent.x;
				this->w = parent.w;
			}
			if (CHECKFLAG(layout, rectlayout::fillheight))
			{
				this->y = parent.y;
				this->h = parent.h;
			}
		}
		// Half width
		else if (ANYFLAG(layout, rectlayout::halfsize))
		{
			if (CHECKFLAG(layout, rectlayout::halfwidth))
			{
				this->w = parent.w / 2.0f;
			}
			if (CHECKFLAG(layout, rectlayout::halfheight))
			{
				this->h = parent.h / 2.0f;
			}
		}
	}

	// Check anchors in layout
	if (ANYFLAG(layout, 511))
	{
		// Check top
		if (ANYFLAG(layout, 7))
		{
			if (CHECKFLAG(layout, rectlayout::topleft))
			{
				this->setanchor(rectanchor::topleft, parent.getanchor(rectanchor::topleft));
			}
			else if (CHECKFLAG(layout, rectlayout::topcenter))
			{
				this->setanchor(rectanchor::topcenter, parent.getanchor(rectanchor::topcenter));
			}
			else if (CHECKFLAG(layout, rectlayout::topright))
			{
				this->setanchor(rectanchor::topright, parent.getanchor(rectanchor::topright));
			}
		}
		// Middle
		else if (ANYFLAG(layout, 56))
		{
			if (CHECKFLAG(layout, rectlayout::midleft))
			{
				this->setanchor(rectanchor::midleft, parent.getanchor(rectanchor::midleft));
			}
			else if (CHECKFLAG(layout, rectlayout::midcenter))
			{
				this->setanchor(rectanchor::midcenter, parent.getanchor(rectanchor::midcenter));
			}
			else if (CHECKFLAG(layout, rectlayout::midright))
			{
				this->setanchor(rectanchor::midright, parent.getanchor(rectanchor::midright));
			}
		}
		// Bottom
		else if (ANYFLAG(layout, 448))
		{
			if (CHECKFLAG(layout, rectlayout::bottomleft))
			{
				this->setanchor(rectanchor::bottomleft, parent.getanchor(rectanchor::bottomleft));
			}
			else if (CHECKFLAG(layout, rectlayout::bottomcenter))
			{
				this->setanchor(rectanchor::bottomcenter, parent.getanchor(rectanchor::bottomcenter));
			}
			else if (CHECKFLAG(layout, rectlayout::bottomright))
			{
				this->setanchor(rectanchor::bottomright, parent.getanchor(rectanchor::bottomright));
			}
		}
	}
}

void rectf::setanchor(rectanchor anchor, glm::vec2 pos)
{
	if (ANYFLAG(anchor, 511))
	{
		// Check top
		if (ANYFLAG(anchor, 7))
		{
			if (CHECKFLAG(anchor, rectlayout::topleft))
			{
				//return glm::ivec2(this->x, this->y);
				this->x = pos.x;
				this->y = pos.y;
			}
			else if (CHECKFLAG(anchor, rectlayout::topcenter))
			{
				this->x = pos.x - (this->w / 2.0f);
				this->y = pos.y;
				//return glm::ivec2(this->x + (this->w / 2), this->y);
			}
			else if (CHECKFLAG(anchor, rectlayout::topright))
			{
				this->x = pos.x - this->w;
				this->y = pos.y;
				//return glm::ivec2(this->x + this->w, this->y);
			}
		}
		// Middle
		else if (ANYFLAG(anchor, 56))
		{
			if (CHECKFLAG(anchor, rectlayout::midleft))
			{
				this->x = pos.x;
				this->y = pos.y - (this->h / 2.0f);
				//return glm::ivec2(this->x, this->y + (this->h / 2));
			}
			else if (CHECKFLAG(anchor, rectlayout::midcenter))
			{
				this->x = pos.x - (this->w / 2.0f);
				this->y = pos.y - (this->h / 2.0f);
				//return glm::ivec2(this->x + (this->w / 2), this->y + (this->h / 2));
			}
			else if (CHECKFLAG(anchor, rectlayout::midright))
			{
				this->x = pos.x - this->w;
				this->y = pos.y - (this->h / 2.0f);
				//return glm::ivec2(this->x + this->w, this->y + (this->h / 2));
			}
		}
		// Bottom
		else if (ANYFLAG(anchor, 448))
		{
			if (CHECKFLAG(anchor, rectlayout::bottomleft))
			{
				this->x = pos.x;
				this->y = pos.y - this->h;
			}
			else if (CHECKFLAG(anchor, rectlayout::bottomcenter))
			{
				this->x = pos.x - (this->w / 2.0f);
				this->y = pos.y - this->h;
			}
			else if (CHECKFLAG(anchor, rectlayout::bottomright))
			{
				this->x = pos.x - this->w;
				this->y = pos.y - this->h;
			}
		}
	}
}

void rectf::inflate(float amount)
{
	this->x -= amount;
	this->y -= amount;
	this->w += (amount * 2.0f);
	this->h += (amount * 2.0f);
}

void rectf::restructure(float x, float y, float width, float height)
{
	this->x = x;
	this->y = y;
	this->w = width;
	this->h = height;
}

bool rectf::intersects(rectf other)
{
	return
		this->x < (other.x + other.w) &&
		other.x < (this->x + this->w) &&
		this->y < (other.y + other.h) &&
		other.y < (this->y + this->h);
}

bool rectf::contains(float x, float y)
{
	if (this->w <= 0.0f && this->h <= 0.0f)
		return false;
	return
		x >= this->x &&
		x < (this->x + this->w) &&
		y >= this->y &&
		y < (this->y + this->h);
}

#pragma endregion [float]

#pragma endregion [Rectangle]
