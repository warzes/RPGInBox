#pragma once

#include "IGameModule.h"

class NewGameScene final : public IGameModule
{
public:
	static IGameModule* Make();
private:

};