#include "stdafx.h"
#include "BoneObject.h"

BoneObject::BoneObject(BaseMaterial* pMaterial, float length)
	: m_pMaterial{ pMaterial }
	, m_Length{ length }
{
}

void BoneObject::AddBone(BoneObject* pBone)
{
	pBone->GetTransform()->Translate(0.f, 0.f, -m_Length);
	AddChild(pBone);
}

void BoneObject::Initialize(const SceneContext&)
{
	GameObject* pEmpty = new GameObject();
	AddChild(pEmpty);
	ModelComponent* pModel{ new ModelComponent{ L"Resources/Meshes/Bone.ovm" } };
	pModel->SetMaterial(m_pMaterial);
	pEmpty->GetTransform()->Rotate(0,-90,0);
	pEmpty->GetTransform()->Scale(m_Length, m_Length, m_Length);
}
