#include "stdafx.h"
#include "WinBolt.h"

#include "Character.h"

#include "Materials/ColorMaterial.h"

WinBolt::WinBolt()
	:GameObject{}
{
}

void WinBolt::Initialize(const SceneContext&)
{
	m_pRigid = AddComponent(new RigidBodyComponent(true));

	float scale{ .4f };
	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	m_pRigid->AddCollider(PxBoxGeometry{ scale,scale,scale * 2.f }, *pDefaultMaterial, true);

	const auto pMat = MaterialManager::Get()->CreateMaterial<ColorMaterial>();
	pMat->SetColor(DirectX::XMFLOAT4{ DirectX::Colors::Gold });

	//Character Mesh
	//**************
	const auto visuals = AddChild(new GameObject);
	const auto pModel = visuals->AddComponent(new ModelComponent(L"Meshes/Bullet.ovm"));
	pModel->SetMaterial(pMat);
	visuals->GetTransform()->Scale(scale, scale, scale);

	SetOnTriggerCallBack([=](GameObject* pTriggerObject, GameObject* pOtherObject, PxTriggerAction action)
		{
			OnHit(pTriggerObject, pOtherObject, action);
		});
}

void WinBolt::Update(const SceneContext&)
{
}

void WinBolt::OnHit(GameObject* , GameObject* pOtherObject, PxTriggerAction action)
{
	if (action == PxTriggerAction::ENTER)
	{
		Character* pCharacter = dynamic_cast<Character*>(pOtherObject);
		if (pCharacter != nullptr)
		{
			pCharacter->SetDidIWin(true);
			return;
		}
	}
}
