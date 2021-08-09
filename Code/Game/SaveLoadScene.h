#pragma once

#include "IGameModule.h"

class SaveLoadScene final : public IGameModule
{
public:
	static IGameModule* MakeSave();
	static IGameModule* MakeLoad();
private:

};