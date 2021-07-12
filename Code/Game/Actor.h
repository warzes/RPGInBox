#pragma once
#if ASCII_DEFINE
#include "Handle.h"

typedef void (*ActorTakeTurnCallback)(Handle actor);
typedef void (*ActorRoundEndCallback)(Handle actor);

void InitActors(int capacity);
int MaxActors(void);

bool IsActorValid(Handle actor);
Handle SpawnActor(ActorTakeTurnCallback onTakeTurn, ActorRoundEndCallback onRoundEnd); // If either callback is NULL, an crash occurs as intended
void DestroyActor(Handle actor);
void DestroyAllActors(void);
int CountActors(void);

int GetActorTurnCooldown(Handle actor);
bool IsActorsTurn(Handle actor);                        // Returns whether it is the actor's turn, based on the sort from the last ProcessActorTurn call
bool ProcessActorTurn(void);                            // Return true if an actor still has a turn to take, or false if a new round is neccessary
bool ProcessActorRound(void);                           // Returns true if another round can be processed, and false if an actor can take a turn
void SetActorTurnCooldown(Handle actor, int cooldown);  // Set the actors cooldown, but order is not applied until ProcessActorTurn is called

#endif