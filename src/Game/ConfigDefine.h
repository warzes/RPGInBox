#pragma once

constexpr auto MaxNumberState = 999;
constexpr auto HeroPartySize = 4u;
constexpr auto MaxEnemyPartySize = 6u;

constexpr auto MaxNumberCellInBattle = 3u;
constexpr auto MaxNumberLineInBattle = 4u;
constexpr auto MaxNumberHalfLineInBattle = MaxNumberLineInBattle/2u;

#define TURN_STEP 1

#define MAIN_FRAME_TO_RENDER_TEXTURE 0
#define OLD_SCHOOL_RENDER 0