#include "stdafx.h"
#include "SoftwareSkinningScene_1.h"
#include "Managers/MaterialManager.h"
#include "Prefabs/BoneObject.h"
#include "../OverlordProject/Materials/ColorMaterial.h"

void SoftwareSkinningScene_1::Initialize()
{
	ColorMaterial *pMaterial{ new ColorMaterial{} };

	GameObject *pRoot{ new GameObject() };
	m_pBone0 = new BoneObject{pMaterial, 15.f };
	m_pBone1 = new BoneObject{ pMaterial, 15.f };
	pRoot->AddChild(m_pBone0);
	m_pBone0->AddBone(m_pBone1);

	//pRoot->GetTransform()->Rotate(0.f, -90.f, 0.f);
	AddChild(pRoot);
}

void SoftwareSkinningScene_1::Update()
{
	m_BoneRotation += float(m_RotationSign) * 45.f * m_SceneContext.pGameTime->GetElapsed();
	m_pBone0->GetTransform()->Rotate(m_BoneRotation, 0.f, 0.f);
	m_pBone1->GetTransform()->Rotate(-2.f * m_BoneRotation, 0.f, 0.f);
	if ((45.f <= m_BoneRotation) || (m_BoneRotation <= -45.f))
		m_RotationSign *= -1;
}

void SoftwareSkinningScene_1::OnGUI()
{
}