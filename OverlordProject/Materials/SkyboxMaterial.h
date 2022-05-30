#pragma once
class TextureData;
class SkyboxMaterial final : public Material<SkyboxMaterial>
{
public:
	SkyboxMaterial();
	~SkyboxMaterial() override = default;

	SkyboxMaterial(const SkyboxMaterial& other) = delete;
	SkyboxMaterial(SkyboxMaterial&& other) noexcept = delete;
	SkyboxMaterial& operator=(const SkyboxMaterial& other) = delete;
	SkyboxMaterial& operator=(SkyboxMaterial&& other) noexcept = delete;

	void SetCubemapTexture(const std::wstring& assetFile);
protected:
	void InitializeEffectVariables()override {};
	void OnUpdateModelVariables(const SceneContext& sceneContext, const ModelComponent* pModelComponent)const override;

	TextureData* m_pCubeMapTexture;
};