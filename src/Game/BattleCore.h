#pragma once

#include "EngineMath.h"

constexpr Point2 LeftTopCoordCells = { 262, 44 };
constexpr Point2 OffsetCoordCells = { 10, 10 };
constexpr Point2 SizeCoordCells = { 160, 160 };

// позиции команд игрока
constexpr Point2 LeftTopCoordPlayerCommand = { 810, 415 };
constexpr Point2 SizeCoordPlayerCommand = { 140, 40 };


constexpr std::array<const char*, 4> PlayerActionMainMenu = { "Attack" , "Skill", "Magic", "Defence" };

constexpr std::array<const char*, 3> PlayerActionMainMenu_Attack = { "Melee Attack" , "Shoot", "Cancel"};