#include "stdafx.h"
#include "Sprite.h"

const int DefaultTimer = 8;


Sprite::Sprite(std::shared_ptr<Texture> bitmap)
	: bmp(bitmap),
	x(0),
	y(0),
	flash(false),
	frame(0),
	frameTime(DefaultTimer),
	bmpFlags(0)
{
}

void Sprite::SetFrames(const Bounds* frames, int yOffset)
{
	for (int i = 0; i < 2; i++)
	{
		this->frames[i].X = frames[i].X;
		this->frames[i].Y = frames[i].Y;
		this->frames[i].Y += yOffset;
		this->frames[i].Width = frames[i].Width;
		this->frames[i].Height = frames[i].Height;
	}

	frameTime = DefaultTimer;
	frame = 0;
}

void Sprite::SetFrames(const Bounds16* frames, int yOffset)
{
	memcpy(this->frames, frames, sizeof this->frames);

	for (int i = 0; i < 2; i++)
	{
		this->frames[i].Y += yOffset;
	}

	frameTime = DefaultTimer;
	frame = 0;
}

void Sprite::SetBitmapFlags(int bmpFlags)
{
	this->bmpFlags = bmpFlags;
}

void Sprite::SetTimer(int timer)
{
	frameTime = timer;
}

void Sprite::SetX(int x)
{
	this->x = x;
}

void Sprite::SetY(int y)
{
	this->y = y;
}

int Sprite::GetX()
{
	return x;
}

int Sprite::GetY()
{
	return y;
}

int Sprite::GetFlags()
{
	return bmpFlags;
}

void Sprite::Flash(bool enable)
{
	flash = enable;
}

void Sprite::Update()
{
	if (frameTime == 0)
	{
		frame = (frame + 1) % 2;
		frameTime = DefaultTimer;
	}

	frameTime--;
}

void Sprite::Draw()
{
	DrawAt(x, y);
}

void Sprite::DrawAt(float screenX, float screenY)
{
	if (flash && frameTime < 4)
		return;

	int flip = bmpFlags == 0 ? 1 : -1;

	DrawTexturePro(*bmp,
		{ (float)frames[frame].X, (float)frames[frame].Y, (float)frames[frame].Width, (float)frames[frame].Height },
		{ screenX, screenY, (float)frames[frame].Width * bmpFlags, (float)frames[frame].Height },
		{ 0.0f, 0.0f }, 0.0f, WHITE);
}