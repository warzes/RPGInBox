#pragma once

struct CheckResult
{
	int Message;
	bool Fight;
	int FormationId;
	bool Teleport;
	int TeleportId;     // only level teleports are supported
	const char* ItemName;
};

typedef uint8_t CheckParams[4];

typedef void (*CheckRoutine)(CheckParams params, CheckResult& result);

const int ObjectTypes = 216;


CheckRoutine GetObjectRoutine(int type);