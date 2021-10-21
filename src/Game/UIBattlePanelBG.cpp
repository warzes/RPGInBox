#include "stdafx.h"
#include "UIBattlePanelBG.h"
#include "ResourceManager.h"
#include "DebugNew.h"
//-----------------------------------------------------------------------------
constexpr NPatchInfo ninePatchInfo = { { 0.0f, 0.0f, 64.0f, 64.0f }, 6, 6, 6, 6, NPATCH_NINE_PATCH };
//-----------------------------------------------------------------------------
bool UIBattlePanelBG::Init(ResourceManager* resourceMgr) noexcept
{
	m_patchTexture = resourceMgr->GetTexture("../data/ui/battleui_background.png");
	return true;
}
//-----------------------------------------------------------------------------
void UIBattlePanelBG::Draw() const noexcept
{
	const Rectangle dest = { 20.0f, 35.0f, (float)GetScreenWidth() - dest.x * 2.0f, (float)GetScreenHeight() - dest.y * 2.0f };
	DrawTextureNPatch(*m_patchTexture, ninePatchInfo, dest, { 0.0f, 0.0f }, 0.0f, { 255, 255, 255, 150 });
}
//-----------------------------------------------------------------------------