#include "stdafx.h"
#include "SoftwareSkinningScene_1.h"
#include "Managers/MaterialManager.h"
#include "Prefabs/BoneObject.h"
#include "../OverlordProject/Materials/ColorMaterial.h"

void SoftwareSkinningScene_1::Initialize()
{
	m_SceneContext.settings.enableOnGUI = true;

	ColorMaterial *pMaterial = MaterialManager::Get()->CreateMaterial<ColorMaterial>();

	GameObject *pRoot{ new GameObject() };
	m_pBone0 = new BoneObject{pMaterial, 15.f };
	m_pBone1 = new BoneObject{ pMaterial, 15.f };
	pRoot->AddChild(m_pBone0);
	m_pBone0->AddBone(m_pBone1);

	AddChild(pRoot);
}

void SoftwareSkinningScene_1::Update()
{
	if (m_AutoRotate)
	{
		m_BoneRotation += float(m_RotationSign) * 45.f * m_SceneContext.pGameTime->GetElapsed();
		m_pBone0->GetTransform()->Rotate(0.f, 0.f, m_BoneRotation);
		m_pBone1->GetTransform()->Rotate(0.f, 0.f, -2.f * m_BoneRotation);

		if ((45.f <= m_BoneRotation) || (m_BoneRotation <= -45.f))	m_RotationSign *= -1;
	}
	else
	{
		m_pBone0->GetTransform()->Rotate(m_Bone0Rotation);
		m_pBone1->GetTransform()->Rotate(m_Bone1Rotation);
	}
}

void SoftwareSkinningScene_1::OnGUI()
{
	if (ImGui::CollapsingHeader("Bones"))
	{
		ImGui::Checkbox("Auto Rotate", &m_AutoRotate);
		ImGui::SliderFloat3("Rotation Bone 0", &m_Bone0Rotation.x,-45.f,45.f);
		ImGui::SliderFloat3("Rotation Bone 1", &m_Bone1Rotation.x,-45.f,45.f);
	}
}