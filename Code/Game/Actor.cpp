#include "stdafx.h"
#include "Actor.h"

static void SortActorTurnOrder(void);

static int actorCapacity;
static int actorCount;
static int actorLowestFree;
static struct ActorStatus {
	bool reserved;
	unsigned int version;
} *actorStatus;
static struct ActorData {
	int turnCooldown;
	ActorTakeTurnCallback onTakeTurn;
	ActorRoundEndCallback onTurnEnd;
} *actorData;
static Handle* actorOrder;

void InitActors(int capacity)
{
	int i;

	if (capacity < 1)
		capacity = 1;

	actorStatus = (ActorStatus*)MemAlloc(capacity * sizeof(*actorStatus));
	actorData = (ActorData*)MemAlloc(capacity * sizeof(*actorData));
	actorOrder = (Handle * )MemAlloc(capacity * sizeof(*actorOrder));

	// CRASH: Allocation failure. Note, if we run out of paging file space 
	// it will hard crash before this, and probably with an ambiguous error
	if (!actorStatus || !actorData || !actorOrder) {
		TraceLog(LOG_ERROR, TextFormat("ACTOR: Failed to allocate module to a capacity of %d", capacity));
		return;
	}

	actorCapacity = capacity;

	for (i = 0; i < actorCapacity; i++) {
		actorStatus[i].reserved = false;
		actorStatus[i].version = 0;
	}

	// Primarily ensure no unexpected callback behaviour. We want a crash 
	// when NULL callbacks are called, and not a random function
	for (i = 0; i < actorCapacity; i++) {
		actorData[i].turnCooldown = 0;
		actorData[i].onTakeTurn = NULL;
		actorData[i].onTurnEnd = NULL;
	}

	// Ensure we avoid any wierd sorting behaviour due to garbage values
	for (i = 0; i < actorCapacity; i++)
		actorOrder[i] = NULL_HANDLE;

	TraceLog(LOG_INFO, TextFormat("ACTOR: Initialized successfully (capacity: %d)", actorCapacity));
}

inline int MaxActors(void)
{
	return actorCapacity;
}

inline bool IsActorValid(Handle actor)
{
	return !IsHandleNull(actor)
		&& actorStatus[actor.index].reserved
		&& actorStatus[actor.index].version == actor.version;
}

Handle SpawnActor(ActorTakeTurnCallback onTakeTurn, ActorRoundEndCallback onTurnEnd)
{
	int i;

	for (i = actorLowestFree; i < actorCapacity; i++) {
		if (!actorStatus[i].reserved) {
			Handle handle;

			// Update instance status
			actorStatus[i].reserved = true;
			actorStatus[i].version++;

			// Make handle
			handle.version = actorStatus[i].version;
			handle.index = i;

			// Update module status
			actorCount++;
			actorLowestFree = i + 1;

			// Prepare instance
			actorData[i].turnCooldown = 0;
			actorData[i].onTakeTurn = onTakeTurn;
			actorData[i].onTurnEnd = onTurnEnd;

			// Append to actor order and sort
			actorOrder[actorCount - 1] = handle;

			return handle;
		}
	}

	return NULL_HANDLE;
}

void DestroyActor(Handle actor)
{
	int i;
	int j;

	// An unfortunately necessary and costly operation because we can't  
	// reliably cache the actor's order index. Pray we don't have hundreds 
	// of actors to destroy in a single frame!
	for (i = 0; i < actorCount; i++) {
		if (AreHandlesEqual(actorOrder[i], actor)) {
			for (j = i; j < actorCount - 1; j++)
				actorOrder[j] = actorOrder[j + 1];
		}
	}

	actorCount--;
	actorStatus[actor.index].reserved = false;

	if (actor.index < actorLowestFree)
		actorLowestFree = actor.index;
}

void DestroyAllActors(void)
{
	int i;

	for (i = 0; i < actorCapacity; i++)
		actorStatus[i].reserved = false;

	for (i = 0; i < actorCount; i++)
		actorOrder[i] = NULL_HANDLE;

	actorCount = 0;
	actorLowestFree = 0;
}

inline int CountActors(void)
{
	return actorCount;
}

inline int GetActorTurnCooldown(Handle actor)
{
	return actorData[actor.index].turnCooldown;
}

inline bool IsActorsTurn(Handle actor)
{
	return IsActorValid(actorOrder[0])
		&& AreHandlesEqual(actorOrder[0], actor);
}

bool ProcessActorTurn(void)
{
	if (actorCount == 0)
		return false;

	SortActorTurnOrder();

	if (actorCount > 0)
		actorData[actorOrder[0].index].onTakeTurn(actorOrder[0]);

	// TODO: Maybe divide into ShouldProcessActorTurn
	return actorCount > 0 && actorData[actorOrder[0].index].turnCooldown <= 0;
}

bool ProcessActorRound(void)
{
	if (actorCount == 0)
		return false;

	SortActorTurnOrder();

	// If a new round has begun (the first actor in order is cooling down), 
	// tick down all actors, and end their turns. Then, return NULL_HANDLE 
	// to signify this round is over, with no actor needing to take a turn
	if (actorData[actorOrder[0].index].turnCooldown > 0) {
		int i;

		// Handle turns in reverse, so that destroyed actors don't  
		// sabotage the list. TODO: probably best if we have a toggle 
		// and destroy request queue; that approach would also faster 
		// because we only have to clear the list once
		for (i = actorCount - 1; i >= 0; i--) {
			actorData[actorOrder[i].index].turnCooldown--;
			actorData[actorOrder[i].index].onTurnEnd(actorOrder[i]);
		}
	}

	// TODO: Maybe divide into ShouldProcessActorRound
	return actorCount > 0 && actorData[actorOrder[0].index].turnCooldown > 0;
}

inline void SetActorTurnCooldown(Handle actor, int cooldown)
{
	actorData[actor.index].turnCooldown = cooldown;
}

// Insertion sort used because the list is likely small and mostly, if not 
// entirely, sorted
static void SortActorTurnOrder(void)
{
	int i;
	int j;
	Handle currentActor;

	for (i = 1; i < actorCount; i++) {
		j = i - 1;
		currentActor = actorOrder[i];

		while (j >= 0 && actorData[actorOrder[j].index].turnCooldown > actorData[currentActor.index].turnCooldown) {
			actorOrder[j + 1] = actorOrder[j];
			j = j - 1;
		}

		actorOrder[j + 1] = currentActor;
	}
}