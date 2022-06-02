#include "stdafx.h"
#include "BoltPicksUp.h"
#include "UI/HUD.h"

#include "Materials/ColorMaterial.h"


BoltPicksUp::BoltPicksUp()
	: GameObject{}
{
}

void BoltPicksUp::Initialize(const SceneContext&)
{
	m_pRigid = AddComponent(new RigidBodyComponent());
	m_pRigid->SetKinematic(true);

	float scale{ .4f };
	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	m_pRigid->AddCollider(PxBoxGeometry{ scale,scale,scale * 2.f }, *pDefaultMaterial, true);

	const auto pMat = MaterialManager::Get()->CreateMaterial<ColorMaterial>();
	pMat->SetColor(DirectX::XMFLOAT4{ (GetTag() == L"Enemy") ? DirectX::Colors::DarkRed : DirectX::Colors::DarkBlue });

	//Character Mesh
	//**************
	const auto visuals = AddChild(new GameObject);
	const auto pModel = visuals->AddComponent(new ModelComponent(L"Meshes/Bolt.ovm"));
	pModel->SetMaterial(pMat);
	visuals->GetTransform()->Scale(scale, scale, scale);

	SetOnTriggerCallBack([=](GameObject* pTriggerObject, GameObject* pOtherObject, PxTriggerAction action)
		{
			OnHit(pTriggerObject, pOtherObject, action);
		});

}

void BoltPicksUp::Update(const SceneContext&)
{
}

void BoltPicksUp::OnHit(GameObject* pTriggerObject, GameObject* pOtherObject, PxTriggerAction action)
{
	if (pOtherObject->GetTag() == L"Friendly")
	{
		HUD::Get()->IncreaseBolts(m_Score);
	}
}
