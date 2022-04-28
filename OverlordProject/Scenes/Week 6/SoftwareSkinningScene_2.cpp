#include "stdafx.h"
#include "SoftwareSkinningScene_2.h"
#include "Managers/MaterialManager.h"
#include "Prefabs/BoneObject.h"
#include "../OverlordProject/Materials/ColorMaterial.h"

void SoftwareSkinningScene_2::Initialize()
{
	ColorMaterial* pMaterial = MaterialManager::Get()->CreateMaterial<ColorMaterial>();

	GameObject* pRoot{ new GameObject() };
	m_pBone0 = new BoneObject{ pMaterial, 15.f };
	m_pBone1 = new BoneObject{ pMaterial, 15.f };
	pRoot->AddChild(m_pBone0);
	m_pBone0->AddBone(m_pBone1);

	AddChild(pRoot);
}

void SoftwareSkinningScene_2::Update()
{
	m_BoneRotation += float(m_RotationSign) * 45.f * m_SceneContext.pGameTime->GetElapsed();
	m_pBone0->GetTransform()->Rotate(0.f, 0.f, m_BoneRotation);
	m_pBone1->GetTransform()->Rotate(0.f, 0.f, -2.f * m_BoneRotation);

	if ((45.f <= m_BoneRotation) || (m_BoneRotation <= -45.f))	m_RotationSign *= -1;
}

void SoftwareSkinningScene_2::OnGUI()
{
}