#pragma once

#include "IGameModule.h"

class Overworld final : public IGameModule, public IPlayfield
{
    static const int TileTypes = 128;
    static const int RowCount = 256;
    static const int ColCount = 256;
    static const int TileWidth = 16;
    static const int TileHeight = 16;
    static const int VisibleRows = 16;
    static const int VisibleCols = 17;
    static const int MiddleRow = 7;
    static const int MiddleCol = 7;
    static const int EnterTeleports = 32;

    typedef void (Overworld::* UpdateFunc)();

    static Overworld* instance;

public:
    Overworld() noexcept;
    ~Overworld();

    void Init(int startCol, int startRow) noexcept;

    void Update() noexcept final;
    void Draw() noexcept final;

    IPlayfield* AsPlayfield() noexcept final;

    static IGameModule* Make() noexcept;

    static Point2 GetPlayerPos();
};