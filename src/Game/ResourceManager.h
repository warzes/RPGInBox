#pragma once

// хранилище ассетов (текстуры, модели, звуки и т.д.) игры
class ResourceManager final : NonCopyable, NonMovable
{
public:
	~ResourceManager();

	bool Init() noexcept;

	std::shared_ptr<Texture2D> GetTexture(const std::string& name) noexcept;

	//Shader GetCoreWorldShader() noexcept;

private:
	std::unordered_map<std::string, std::shared_ptr<Texture2D>> m_textures;
	Shader m_coreWorldShader;
};