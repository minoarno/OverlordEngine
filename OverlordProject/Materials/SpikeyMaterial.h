#pragma once
#include "Misc/Material.h"

class TextureData;

class SpikeyMaterial final: public Material<SpikeyMaterial>
{
public:
	SpikeyMaterial();
	~SpikeyMaterial();

	void SetDiffuseTexture(const std::wstring& assetFile);

protected:
	void InitializeEffectVariables()override;
	void OnUpdateModelVariables(const SceneContext& sceneContext, const ModelComponent* pModelComponent)const override;

	TextureData* m_pDiffuseTexture;
	static ID3DX11EffectShaderResourceVariable* m_pDiffuseSRVvariable;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	SpikeyMaterial(const SpikeyMaterial &obj);
	SpikeyMaterial& operator=(const SpikeyMaterial& obj);
};

