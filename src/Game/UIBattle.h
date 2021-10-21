#pragma once

#include "UIBattlePanelBG.h"
#include "UIBattlePlayerMenu.h"

class ResourceManager;

// пункты меню
struct PlayerMenuLabel { enum { Attack, Skill, Magic, Defence }; };
constexpr std::array PlayerActionMainMenu{ "Attack", "Skill", "Magic", "Defence" };
static_assert(PlayerActionMainMenu.size() == 4);
struct PlayerMenuAttackLabel { enum { Melee, Shoot }; };
constexpr std::array PlayerActionMainMenu_Attack = { "Melee", "Shoot" };
static_assert(PlayerActionMainMenu_Attack.size() == 2);

class UIBattle final
{
public:
	UIBattle(ResourceManager& resourceMgr) noexcept;

	bool Init() noexcept;

	void Update() noexcept;
	void Draw() const noexcept;

	// возвращает -1 если не выбрано меню
	int SelectMenu() const noexcept;

	void ResetPlayerMenu() noexcept;
	void ActivePlayerMainMenu() noexcept;
	void ActivePlayerMenuAttack() noexcept;

private:
	void drawPanels() const noexcept;

	ResourceManager& m_resourceMgr;

	UIBattlePlayerMenu m_playerMenu;
	UIBattlePlayerMenu m_playerMenu_attack;
	UIBattlePlayerMenu* m_currentPlayerMenu = nullptr;

	UIBattlePanelBG m_background;
	std::shared_ptr<Texture2D> m_battleBackGround = nullptr;
};