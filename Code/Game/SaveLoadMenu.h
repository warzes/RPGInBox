#pragma once

#include "GameMenu.h"
#include "SaveFolder.h"

class SaveLoadMenu : public Menu
{
public:
    enum Action
    {
        Save,
        Load,
    };

private:
    Action action;
    int selIndex;
    int topRow;
    SummarySet summaries;
    bool hasError;

    std::shared_ptr<Texture2D> m_patchTexture = nullptr;
    std::shared_ptr<Texture> m_cursor = nullptr;

public:
    SaveLoadMenu(Action action);

    virtual MenuAction Update(Menu*& nextMenu);
    virtual void Draw(MenuDrawState state);

private:
    MenuAction Close();
    void DrawFile(int slot, int boxY);
    void DrawErrorMessage();
};