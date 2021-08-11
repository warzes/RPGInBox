#pragma once

#include "MapSprite.h"
#include "Sprite.h"

class VehicleSprite : public IMapSprite
{
	Sprite sprite;
	bool moving;
	Direction direction;
	int frameOffsetY;

public:
	VehicleSprite(std::shared_ptr<Texture> bmp, int frameOffsetY);

	virtual int GetX();
	virtual int GetY();

	virtual void SetX(int x);
	virtual void SetY(int y);

	virtual Direction GetDirection();
	virtual void SetDirection(Direction direction);

	virtual void Start();
	virtual void Stop();

	virtual void Update();
	virtual void DrawAt(float screenX, float screenY);

private:
	void SetFrames();
};


class CanoeSprite : public VehicleSprite
{
public:
	CanoeSprite(std::shared_ptr<Texture> bmp);
};


class ShipSprite : public VehicleSprite
{
public:
	ShipSprite(std::shared_ptr<Texture> bmp);

	virtual void Start();
	virtual void Stop();
};


class AirshipSprite : public IMapSprite
{
public:
	enum State
	{
		Lifting,
		Landing,
	};

private:
	static const int FrameOffsetY = 2 * 16;
	static const int LiftHeight = 2 * 16;
	static const int LiftSpeed = 1;

	Sprite sprite;
	Direction direction;
	std::shared_ptr<Texture> bitmap;

	int x;
	int y;
	int liftY;
	State state;

public:
	AirshipSprite(std::shared_ptr<Texture> bmp);

	virtual int GetX();
	virtual int GetY();

	virtual void SetX(int x);
	virtual void SetY(int y);

	virtual Direction GetDirection();
	virtual void SetDirection(Direction direction);

	virtual void Start();
	virtual void Stop();

	virtual void Update();
	virtual void DrawAt(float screenX, float screenY);

	void SetState(State state);
	bool FinishedLiftLand();

private:
	void SetFrames();
};