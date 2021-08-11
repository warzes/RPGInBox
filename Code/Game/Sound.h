#pragma once

enum SoundId
{
    Sound_Prelude,
    Sound_Opening,
    Sound_Ending,
    Sound_Field,
    Sound_Ship,
    Sound_Airship,
    Sound_Town,
    Sound_Castle,
    Sound_Volcano,
    Sound_Matoya,
    Sound_Dungeon,
    Sound_Temple,
    Sound_Sky,
    Sound_PastTemple,
    Sound_Shop,
    Sound_Battle,
    Sound_Menu,
    Sound_Dead,
    Sound_Victory,
    Sound_Fanfare,
    Sound_GotItem,
    Sound_Save,
    Sound_Potion,
    Sound_ChaosRumble,
};

enum SEffectId
{
    SEffect_Confirm,
    SEffect_Cursor,
    SEffect_Door,
    SEffect_Error,
    SEffect_Fight,
    SEffect_Hurt,
    SEffect_Magic,
    SEffect_Potion,
    SEffect_Step,
    SEffect_Strike,
    SEffect_Sea,
    SEffect_Lift,
    SEffect_Land,
    SEffect_Airship,
    SEffect_Lava,

    SEffect_Max
};


class SoundManager
{
public:
    static bool Init();
    static void Close();

    static void Update();

    static void PlayTrack(int id);

    static void PlayEffect(int id, bool loop = false);
    static void StopEffect();
};