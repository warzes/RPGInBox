#pragma once

#include "Global.h"

class Sprite
{
    int x;
    int y;
    std::shared_ptr<Texture> bmp;
    Bounds16 frames[2];
    int bmpFlags;
    bool flash;
    int frame;
    int frameTime;

public:
    Sprite(std::shared_ptr<Texture> bitmap);

    int GetX();
    int GetY();
    int GetFlags();

    void SetFrames(const Bounds* frames, int yOffset);
    void SetFrames(const Bounds16* frames, int yOffset);
    void SetBitmapFlags(int bmpFlags);
    void SetTimer(int timer);

    void SetX(int x);
    void SetY(int y);

    void Flash(bool enable);

    void Update();
    void Draw();
    void DrawAt(float screenX, float screenY);
};