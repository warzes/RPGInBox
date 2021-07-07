#pragma once

#include <stddef.h>
#include <string>
#include <vector>
#include <map>
#include <functional>

namespace RLSprites
{
    constexpr int DIRECTION_DEFAULT = 0;
    constexpr int DIRECTION_UP = 0;
    constexpr int DIRECTION_LEFT = 1;
    constexpr int DIRECTION_DOWN = 2;
    constexpr int DIRECTION_RIGHT = 3;

    class SpriteImage
    {
    public:
        std::string ImageSource;
        std::vector<Rectangle> Frames;
        Texture Sheet;
        int StartFrame = 0;
    };

    class SpriteInstance;

    typedef std::function<void(SpriteInstance*, int)> SpriteFrameCallback;

    class SpriteFrameInfo
    {
    public:
        std::string Name;
        SpriteFrameCallback Callback;
    };

    class SpriteAnimation
    {
    public:
        std::string Name;
        float FramesPerSecond = 15;
        bool Loop = false;
        std::map<int, std::vector<int>> DirectionFrames;

        std::map<int, SpriteFrameInfo> FrameCallbacks;

        SpriteAnimation Clone();
        void Reverse();
    };

    class Sprite
    {
    public:
        std::vector<SpriteImage> Images;

        std::map<std::string, SpriteAnimation> Animations;

        int AddImage(Texture tx, int xFrameCount = 1, int yFrameCount = 1, const char* name = nullptr);
        int AddImage(const std::string& imageName, int xFrameCount = 1, int yFrameCount = 1);

        int AddFlipFrames(int startFrame, int endFrame, bool flipHorizontal, bool flipVertical);

        SpriteAnimation* FindAnimation(const std::string& name);

        void AddAnimation(SpriteAnimation& animation);
        SpriteAnimation* AddAnimation(const std::string name, int direction, int start, int end);

        void SetAnimationLoop(const std::string& name, bool loop);
        void SetAnimationSpeed(const std::string& name, float fps);

        void AddAnimationFrameCallback(const std::string& animationName, SpriteFrameCallback callback, const std::string& frameName, int frame = -1);
        void SetAnimationFrameCallback(const std::string& animationName, SpriteFrameCallback callback, const std::string& frameName);

        bool Save(const char* filePath);

        static Sprite Load(const char* filePath);
    };

    enum class OriginLocations
    {
        Minium,
        Center,
        Maximum
    };

    class SpriteInstance
    {
    public:
        class Layer
        {
        public:
            Sprite* Image;
            Color Tint = WHITE;
        };

        std::vector<Layer> Layers;

        Vector2 Position = { 0,0 };
        int Direction = 0;
        float Rotation = 0;
        float Scale = 1.0f;
        float Speed = 1.0f;

        OriginLocations OriginX = OriginLocations::Minium;
        OriginLocations OriginY = OriginLocations::Minium;

        Rectangle LastRectangle = { 0,0,0,0 };

        SpriteAnimation* CurrentAnimation = nullptr;
        int CurrentFrame = -1;
        int CurrentDirection = DIRECTION_DEFAULT;
        int CurrentRealFrame = -1;
        double LastFrameTime = 0;

        std::string TriggerFrameName;

        SpriteInstance(Sprite& sprite, Color tint = WHITE) { Layers.push_back(Layer{ &sprite,tint }); }

        void SetAimation(const std::string& name);

        void Update();
        void Render();
        void UpdateRender();
    };
}