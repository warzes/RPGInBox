#pragma once

#include "IGameModule.h"

class IntroScene final : public IGameModule
{
public:
    IntroScene() = default;

    void Update() noexcept final;
    void Draw() noexcept final;

    IPlayfield* AsPlayfield() noexcept final;

    static IGameModule* Make() noexcept;

};