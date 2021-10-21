#include "stdafx.h"
#include "UIAnimSwords.h"
#include "ResourceManager.h"
#include "DebugNew.h"
//-----------------------------------------------------------------------------
constexpr auto speedAnimation = 300.0f;
//-----------------------------------------------------------------------------
bool UIAnimSwords::Init(ResourceManager& resourceMgr) noexcept
{
	m_texture = resourceMgr.GetTexture("../data/ui/sword.png");
	m_textureRect = { 0.0f, 0.0f, (float)m_texture->width, (float)m_texture->height };
	return true;
}
//-----------------------------------------------------------------------------
void UIAnimSwords::UpdateAndDraw(float deltaTime, const Point2& pos, float scale) noexcept
{
	if (m_runAnim == true)
	{
		m_rot1 -= speedAnimation * deltaTime;
		m_rot2 += speedAnimation * deltaTime;
	}

	const Rectangle dest = 
	{
		(float)pos.x + (float)m_texture->width / 2.0f * scale,
		(float)pos.y + (float)m_texture->height / 2.0f * scale,
		(float)m_texture->width * scale,
		(float)m_texture->height * scale
	};
	const Vector2 origin = { (float)m_texture->width / 2.0f * scale, (float)m_texture->height / 2.0f * scale };
	DrawTexturePro(*m_texture, m_textureRect, dest, origin, m_rot1, WHITE);
	DrawTexturePro(*m_texture, m_textureRect, dest, origin, m_rot2, WHITE);
	if (m_rot1 < -45.0f)
	{
		m_isFinal = true;
		m_runAnim = false;
	}
}
//-----------------------------------------------------------------------------
void UIAnimSwords::Reset() noexcept
{
	m_rot1 = 0.0f;
	m_rot2 = 270.0f;
	m_runAnim = false;
	m_isFinal = false;
}
//-----------------------------------------------------------------------------
void UIAnimSwords::Start() noexcept
{
	m_runAnim = true;
}
//-----------------------------------------------------------------------------
bool UIAnimSwords::IsFinal() const noexcept
{
	return m_isFinal;
}
//-----------------------------------------------------------------------------