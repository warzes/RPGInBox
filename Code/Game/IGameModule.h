#pragma once

class IGameCamera;

class IPlayfield
{
public:
    virtual ~IPlayfield() = default;

    virtual Point2 GetCurrentPos() = 0;
    virtual int GetInRoom() = 0;

    virtual void HandleShopClosed() = 0;
    virtual void HandleFightEnded() = 0;
    virtual void HandleOpeningEnded() = 0;
};

class IGameModule
{
public:
	virtual ~IGameModule() = default;

	virtual void Update() = 0;
	virtual void Draw() = 0;

    virtual IPlayfield* AsPlayfield() = 0;

    // TODO:  static IGameModule* Make()
};