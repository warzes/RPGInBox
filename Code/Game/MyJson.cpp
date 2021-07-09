#include "stdafx.h"
#include "MyJson.h"

bool MyJson::checkValid(Json::Value& value, std::string member)
{
	if (value.isMember(member))
	{
		return true;
	}
	else
	{
		//std::cerr << "json error invalid member " << member << std::endl;
		return false;
	}
}

Json::Value MyJson::get(Json::Value& value, std::string member)
{
	if (checkValid(value, member))
	{
		return value[member];
	}
	else
	{
		return NULL;
	}
}