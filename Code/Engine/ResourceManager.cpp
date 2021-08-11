#include "stdafx.h"
#include "ResourceManager.h"
#include "DebugNew.h"
ResourceManager* thisResourceManager = nullptr;
//-----------------------------------------------------------------------------
ResourceManager::ResourceManager()
{
	thisResourceManager = this;
}
//-----------------------------------------------------------------------------
ResourceManager::~ResourceManager()
{
	for (auto it = m_textures.begin(); it != m_textures.end(); ++it)
		UnloadTexture(*it->second);
}
//-----------------------------------------------------------------------------
std::shared_ptr<Texture2D> ResourceManager::GetTexture(const std::string& name)
{
	if (name == "") return nullptr;

	auto it = m_textures.find(name);
	if (it == m_textures.end())
	{
		Texture2D tex = LoadTexture(name.c_str());
		SetTextureFilter(tex, TEXTURE_FILTER_POINT);
		m_textures[name] = std::make_shared<Texture2D>(std::move(tex));
	}
	return m_textures[name];
}
//-----------------------------------------------------------------------------
std::shared_ptr<Texture2D> ResourceManager::GetGlobalTexture(const std::string& name)
{
	if (!thisResourceManager)
		return nullptr;
	return thisResourceManager->GetTexture(name);
}
//-----------------------------------------------------------------------------