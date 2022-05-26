#include "stdafx.h"
#include "ColorMaterial_Shadow_Skinned.h"

ColorMaterial_Shadow_Skinned::ColorMaterial_Shadow_Skinned()
	: Material(L"Effects/Shadow/PosCol_Shadow_Skinned.fx")
{
}

void ColorMaterial_Shadow_Skinned::SetColor(const DirectX::XMFLOAT4& color)
{
	m_Color = color;
	SetVariable_Vector(L"gColor", m_Color);
}

void ColorMaterial_Shadow_Skinned::InitializeEffectVariables()
{
}

void ColorMaterial_Shadow_Skinned::OnUpdateModelVariables(const SceneContext& sceneContext, const ModelComponent* pModel) const
{

	auto LightWVP = XMMatrixMultiply(XMLoadFloat4x4(&pModel->GetTransform()->GetWorld()), XMLoadFloat4x4(&ShadowMapRenderer::Get()->GetLightVP()));
	SetVariable_Matrix(L"gWorldViewProj_Light", reinterpret_cast<const float*>(LightWVP.r));

	//2. Update the ShadowMap texture
	SetVariable_Texture(L"gShadowMap", ShadowMapRenderer::Get()->GetShadowMap());

	//3. Update the Light Direction (retrieve the direction from the LightManager > sceneContext)
	SetVariable_Vector(L"gLightDirection", sceneContext.pLights->GetDirectionalLight().direction);

	//4. Update the bones
	auto animator = pModel->GetAnimator();
	ASSERT_NULL_(animator);

	SetVariable_MatrixArray(L"gBones",
		(float*)animator->GetBoneTransforms().data(),
		(UINT)animator->GetBoneTransforms().size());
}
