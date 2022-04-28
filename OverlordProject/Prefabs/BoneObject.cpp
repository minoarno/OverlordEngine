#include "stdafx.h"
#include "BoneObject.h"

BoneObject::BoneObject(BaseMaterial* pMaterial, float length)
	: m_pMaterial{ pMaterial }
	, m_Length{ length }
{
}

void BoneObject::AddBone(BoneObject* pBone)
{
	pBone->GetTransform()->Translate(m_Length,0.f, 0.f);
	AddChild(pBone);
}

void BoneObject::CalculateBindPose()
{
	XMStoreFloat4x4(&m_BindPose, XMMatrixInverse(nullptr, XMLoadFloat4x4(&GetTransform()->GetWorld())));
	
	for (BoneObject* pChildBone : GetChildren<BoneObject>())
	{
		pChildBone->CalculateBindPose();
	}
}

void BoneObject::Initialize(const SceneContext&)
{
	GameObject* pEmpty = new GameObject();
	AddChild(pEmpty);
	ModelComponent* pModel{ new ModelComponent{ L"Meshes/Bone.ovm" } };
	pModel->SetMaterial(m_pMaterial);
	pEmpty->GetTransform()->Rotate(0,-90,0);
	pEmpty->GetTransform()->Scale(m_Length, m_Length, m_Length);
	pEmpty->AddComponent(pModel);
}
