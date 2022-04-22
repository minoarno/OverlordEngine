#include "stdafx.h"

#include "SpikeyMaterial.h"

#include "Managers/ContentManager.h"
#include "Misc/TextureData.h"

ID3DX11EffectShaderResourceVariable* SpikeyMaterial::m_pDiffuseSRVvariable = nullptr;

SpikeyMaterial::SpikeyMaterial() : Material(L"Effects/SpikyShader.fx"),
	m_pDiffuseTexture(nullptr)
{}

SpikeyMaterial::~SpikeyMaterial()
{}

void SpikeyMaterial::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"m_TextureDiffuse", m_pDiffuseTexture);
}

void SpikeyMaterial::InitializeEffectVariables()
{
}

void SpikeyMaterial::OnUpdateModelVariables(const SceneContext& sceneContext, const ModelComponent* pModelComponent) const
{
	UNREFERENCED_PARAMETER(sceneContext);
	UNREFERENCED_PARAMETER(pModelComponent);

	if (m_pDiffuseTexture && m_pDiffuseSRVvariable)
	{
		m_pDiffuseSRVvariable->SetResource(m_pDiffuseTexture->GetShaderResourceView());
	}
}