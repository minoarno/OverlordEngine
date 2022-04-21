#pragma once

#include "Misc/Material.h"
class TextureData;
class DiffuseMaterial : public Material<DiffuseMaterial>
{
public:
	DiffuseMaterial();
	~DiffuseMaterial();

	void SetDiffuseMaterial(const std::wstring& assetFile);
protected:
	void InitializeEffectVariables()override;
	void OnUpdateModelVariables(const SceneContext& sceneContext,const ModelComponent* pModelComponent)const override;
private:
	TextureData* m_pDiffuseTexture;
	static ID3DX11EffectShaderResourceVariable* m_pDiffuseSRVvariable;

	DiffuseMaterial(const DiffuseMaterial& other) noexcept = delete;
	DiffuseMaterial& operator=(const DiffuseMaterial& other) noexcept = delete;
	DiffuseMaterial(DiffuseMaterial&& other) noexcept = delete;
	DiffuseMaterial& operator=(DiffuseMaterial&& other) noexcept = delete;
};

