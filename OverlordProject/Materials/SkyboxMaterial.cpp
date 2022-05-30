#include "stdafx.h"
#include "SkyboxMaterial.h"
#include "Misc/TextureData.h"

SkyboxMaterial::SkyboxMaterial()
	: Material(L"Effects/SkyBox.fx")
	, m_pCubeMapTexture{ nullptr }
{
}

void SkyboxMaterial::SetCubemapTexture(const std::wstring& assetFile)
{
	m_pCubeMapTexture = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gCubeMap", m_pCubeMapTexture);
}

void SkyboxMaterial::OnUpdateModelVariables(const SceneContext&, const ModelComponent*) const
{
}
