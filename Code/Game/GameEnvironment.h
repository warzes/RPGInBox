#pragma once

class World;

enum class GameStatus
{
	Exploring, 
	Battle
};

class GameEnvironment
{
public:
	void NextStep(const World& world);

	void EndBattle();

	GameStatus GetStatus() const { return m_status; }

private:
	void setBattle(const World& world);
	unsigned m_dangerLevel = 0;

	GameStatus m_status = GameStatus::Battle;


	// TODO: game time
};