#pragma once

class ResourceManager final : NonCopyable, NonMovable
{
public:
	~ResourceManager();

	std::shared_ptr<Texture2D> GetTexture(const std::string& name);

private:
	std::unordered_map<std::string, std::shared_ptr<Texture2D>> m_textures;
};