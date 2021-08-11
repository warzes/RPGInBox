#include "stdafx.h"
#include "Sound.h"
//-----------------------------------------------------------------------------
static const char* songFiles[] =
{
	"soundff/01_prelude.wav",
	"soundff/02_opening.wav",
	"soundff/03_ending.wav",
	"soundff/04_field.wav",
	"soundff/05_ship.wav",
	"soundff/06_airship.wav",
	"soundff/07_town.wav",
	"soundff/08_castle.wav",
	"soundff/09_volcano.wav",
	"soundff/10_matoya.wav",
	"soundff/11_dungeon.wav",
	"soundff/12_temple.wav",
	"soundff/13_sky.wav",
	"soundff/14_sea_shrine.wav",
	"soundff/15_shop.wav",
	"soundff/16_battle.wav",
	"soundff/17_menu.wav",
	"soundff/18_dead.wav",
	"soundff/19_victory.wav",
	"soundff/20_fanfare.wav",
	"soundff/21_unknown.wav",
	"soundff/22_save.wav",
	"soundff/23_unknown.wav",
	"soundff/24_chaos_rumble.wav",
};
//-----------------------------------------------------------------------------
static const char* effectFiles[SEffect_Max] =
{
	"soundff/ff1-sfx-confirm.wav",
	"soundff/ff1-sfx-cursor.wav",
	"soundff/ff1-sfx-door.wav",
	"soundff/ff1-sfx-error.wav",
	"soundff/ff1-sfx-fight.wav",
	"soundff/ff1-sfx-hurt.wav",
	"soundff/ff1-sfx-magic.wav",
	"soundff/ff1-sfx-potion.wav",
	"soundff/ff1-sfx-step.wav",
	"soundff/ff1-sfx-strike.wav",
	"soundff/ff1-sfx-ship.wav",
	"soundff/ff1-sfx-lift.wav",
	"soundff/ff1-sfx-land.wav",
	"soundff/ff1-sfx-airship.wav",
	"soundff/ff1-sfx-lava.wav",
};

Music currentMusic = {0};
bool isCurrentMusicPlay = false;
Sound effectSamples[SEffect_Max];

bool SoundManager::Init()
{
	for (int i = 0; i < SEffect_Max; i++)
		effectSamples[i] = LoadSound(effectFiles[i]);

	return true;
}

void SoundManager::Close()
{
	for (int i = 0; i < SEffect_Max; i++)
		UnloadSound(effectSamples[i]);

	if (isCurrentMusicPlay)
		UnloadMusicStream(currentMusic);
}

void SoundManager::Update()
{
	if (isCurrentMusicPlay)
		UpdateMusicStream(currentMusic);
}

void SoundManager::PlayTrack(int trackId)
{
	if (trackId < 0 || trackId >= _countof(songFiles))
		return;

	if (isCurrentMusicPlay)
		UnloadMusicStream(currentMusic);
	currentMusic = LoadMusicStream(songFiles[trackId]);
	PlayMusicStream(currentMusic);
	isCurrentMusicPlay = true;
}

void SoundManager::PlayEffect(int id, bool loop)
{
	if (id < 0 || id >= _countof(effectSamples))
		return;

	PlaySound(effectSamples[id]);
}

void SoundManager::StopEffect()
{
}