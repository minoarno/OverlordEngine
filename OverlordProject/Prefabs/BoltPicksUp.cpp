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
	m_pRigid = AddComponent(new RigidBodyComponent(true));

	float scale{ 1.f };
	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	m_pRigid->AddCollider(PxBoxGeometry{ scale,scale,scale }, *pDefaultMaterial, true);

	const auto pMat = MaterialManager::Get()->CreateMaterial<ColorMaterial>();
	pMat->SetColor(DirectX::XMFLOAT4{ DirectX::Colors::DimGray });

	//Character Mesh
	//**************
	const auto visuals = AddChild(new GameObject);
	const auto pModel = visuals->AddComponent(new ModelComponent(L"Meshes/Nut.ovm"));
	pModel->SetMaterial(pMat);
	visuals->GetTransform()->Scale(scale, scale, scale);

	SetOnTriggerCallBack([=](GameObject* pTriggerObject, GameObject* pOtherObject, PxTriggerAction action)
		{
			OnHit(pTriggerObject, pOtherObject, action);
		});

}

void BoltPicksUp::Update(const SceneContext& sceneContext)
{
	m_Rotation += m_RotationSpeed * sceneContext.pGameTime->GetElapsed();
	GetTransform()->Rotate(m_Rotation, 0, 0);

	if (m_FlagForDelete)
	{
		auto activeScene = SceneManager::Get()->GetActiveScene();
		activeScene->RemoveChild(this, true);
	}
}

void BoltPicksUp::OnSceneDetach(GameScene* )
{
	m_FlagForDelete = true;
}

void BoltPicksUp::OnHit(GameObject* , GameObject* pOtherObject, PxTriggerAction)
{

	if (pOtherObject->GetTag() == L"Friendly" && !m_FlagForDelete)
	{
		HUD::Get()->IncreaseBolts(m_Score);

		m_FlagForDelete = true;
	}
}
