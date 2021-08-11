#pragma once

#include "Global.h"

class IMapSprite
{
public:
	virtual int GetX() = 0;
	virtual int GetY() = 0;

	virtual void SetX(int x) = 0;
	virtual void SetY(int y) = 0;

	virtual Direction GetDirection() = 0;
	virtual void SetDirection(Direction direction) = 0;

	virtual void Start() = 0;
	virtual void Stop() = 0;

	virtual void Update() = 0;
	virtual void DrawAt(float screenX, float screenY) = 0;
};


class ICustomMapSprite : public IMapSprite
{
public:
	virtual void SetFrames(int yOffset) = 0;
};


class MapSprite : public ICustomMapSprite
{
	std::shared_ptr<Texture> bmp;
	Direction dir;
	int x;
	int y;
	int frameOffsetY;
	int frame;
	int frameTime;
	bool animating;
	bool showBottom;

public:
	MapSprite(std::shared_ptr<Texture> bitmap);

	int GetX();
	int GetY();

	void SetX(int x);
	void SetY(int y);

	Direction GetDirection();
	void SetDirection(Direction direction);

	void SetFrames(int yOffset);
	void ShowBottom(bool value);

	void Start();
	void Stop();

	void Update();
	void DrawAt(float screenX, float screenY);

private:
	void DrawHorizontalAt(float screenX, float screenY);
	void DrawVerticalAt(float screenX, float screenY);
};
