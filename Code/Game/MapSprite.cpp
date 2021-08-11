#include "stdafx.h"
#include "MapSprite.h"

const int DefaultTimer = 7;

const Bounds walkLeftFrames[2] =
{
{ 2 * 16, 0, 16, 16 },
{ 3 * 16, 0, 16, 16 },
};


MapSprite::MapSprite(std::shared_ptr<Texture> bitmap)
	: bmp(bitmap),
	dir(Dir_Down),
	x(0),
	y(0),
	frameOffsetY(0),
	frame(0),
	frameTime(0),
	animating(false),
	showBottom(true)
{
}

void MapSprite::SetFrames(int yOffset)
{
	frameOffsetY = yOffset;
	frameTime = 0;
	frame = 0;
}

void MapSprite::ShowBottom(bool value)
{
	showBottom = value;
}

void MapSprite::SetDirection(Direction direction)
{
	dir = direction;
}

void MapSprite::SetX(int x)
{
	this->x = x;
}

void MapSprite::SetY(int y)
{
	this->y = y;
}

int MapSprite::GetX()
{
	return x;
}

int MapSprite::GetY()
{
	return y;
}

Direction MapSprite::GetDirection()
{
	return dir;
}

void MapSprite::Start()
{
	animating = true;
}

void MapSprite::Stop()
{
	animating = false;
}

void MapSprite::Update()
{
	if (animating)
	{
		if (frameTime == 0)
		{
			frame = (frame + 1) % 2;
			frameTime = DefaultTimer;
		}
		else
		{
			frameTime--;
		}
	}
}

void MapSprite::DrawAt(float screenX, float screenY)
{
	switch (dir)
	{
	case Dir_Right:
	case Dir_Left:
		DrawHorizontalAt(screenX, screenY);
		break;

	case Dir_Down:
	case Dir_Up:
		DrawVerticalAt(screenX, screenY);
		break;
	}
}

void MapSprite::DrawHorizontalAt(float screenX, float screenY)
{
	int flags = (dir == Dir_Right) ? -1 : 1;
	int height = showBottom ? 16 : 8;

	DrawTexturePro(*bmp, 
		{ (float)walkLeftFrames[frame].X, (float)walkLeftFrames[frame].Y + frameOffsetY, (float)walkLeftFrames[frame].Width, (float)height },
		{ screenX, screenY, (float)walkLeftFrames[frame].Width * flags, (float)height },
		{ 0.0f, 0.0f }, 0.0f, WHITE);
}

void MapSprite::DrawVerticalAt(float screenX, float screenY)
{
	int srcX = (dir == Dir_Up) ? 16 : 0;
	int bottomFlags = (frame == 0) ? 1 : -1;

	DrawTexturePro(*bmp,
		{ (float)srcX, (float)frameOffsetY + 0, 16.0f, 8.0f },
		{ screenX, screenY, 16.0f, 8.0f },
		{ 0.0f, 0.0f }, 0.0f, WHITE);

	if (showBottom)
	{
		DrawTexturePro(*bmp,
			{ (float)srcX, (float)frameOffsetY + 8, 16.0f, 8.0f },
			{ screenX, screenY + 8, 16.0f * bottomFlags, 8.0f },
			{ 0.0f, 0.0f }, 0.0f, WHITE);
	}
}