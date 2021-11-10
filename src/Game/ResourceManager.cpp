#include "stdafx.h"
#include "ResourceManager.h"
#include "DebugNew.h"
//-----------------------------------------------------------------------------
constexpr auto coreWorldShaderVS = R"(
#version 330

in vec3 vertexPosition;
in vec2 vertexTexCoord;
in float vertexColor;

uniform mat4 mvp;

out vec2 fragTexCoord;
out vec4 fragColor;

void main() 
{
	//fragTexCoord = vertexTexCoord / 16;
	//fragColor = vec4(vertexColor / 256, vertexColor / 256, vertexColor / 256, 1.0);
	//vec3 pos = vec3(round(vertexPosition.x / 15.9f), round(vertexPosition.y / 15.9f), round(vertexPosition.z / 15.9f));
	//gl_Position = mvp*vec4(pos, 1.0);
fragTexCoord = vertexTexCoord;
fragColor = vertexColor; 
gl_Position = mvp*vec4(vertexPosition, 1.0);
}
)";

//-----------------------------------------------------------------------------
constexpr auto coreWorldShaderFS = R"(
#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

out vec4 finalColor;

void main()
{
	vec4 texelColor = texture(texture0, fragTexCoord);
	if (texelColor.a <.02) discard;
	finalColor = texelColor*colDiffuse*fragColor;
}
)";

//-----------------------------------------------------------------------------
ResourceManager::~ResourceManager()
{
	for (auto it = m_textures.begin(); it != m_textures.end(); ++it)
		UnloadTexture(*it->second);
}
//-----------------------------------------------------------------------------
bool ResourceManager::Init() noexcept
{
	m_coreWorldShader = LoadShaderFromMemory(coreWorldShaderVS, coreWorldShaderFS);
	return true;
}
//-----------------------------------------------------------------------------
std::shared_ptr<Texture2D> ResourceManager::GetTexture(const std::string& name) noexcept
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
//Shader ResourceManager::GetCoreWorldShader() noexcept
//{
//	return m_coreWorldShader;
//}
//-----------------------------------------------------------------------------