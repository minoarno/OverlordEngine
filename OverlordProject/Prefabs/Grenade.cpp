#include "stdafx.h"
#include "Grenade.h"
#include "Materials/ColorMaterial.h"

#include "Prefabs/Character.h"
#include "Prefabs/RobotEnemy.h"

Grenade::Grenade(const XMFLOAT3& direction)
	: GameObject{}
	, m_Direction{ direction }
{
}

void Grenade::Initialize(const SceneContext&)
{
	m_pRigid = AddComponent(new RigidBodyComponent());
	
	float scale{ .4f };
	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	m_pRigid->AddCollider(PxBoxGeometry{ scale,scale,scale * 2.f }, *pDefaultMaterial);
	m_pRigid->AddCollider(PxBoxGeometry{ m_ExplosionRadius,m_ExplosionRadius,m_ExplosionRadius }, *pDefaultMaterial, true);

	const auto pMat = MaterialManager::Get()->CreateMaterial<ColorMaterial>();
	pMat->SetColor(DirectX::XMFLOAT4{ DirectX::Colors::DarkBlue });

	//Character Mesh
	//**************
	const auto visuals = AddChild(new GameObject);
	const auto pModel = visuals->AddComponent(new ModelComponent(L"Meshes/Grenade.ovm"));
	pModel->SetMaterial(pMat);
	visuals->GetTransform()->Scale(scale, scale, scale);

	m_Func = [=](GameObject* pTriggerObject, GameObject* pOtherObject, PxTriggerAction action)
	{
		OnHit(pTriggerObject, pOtherObject, action);
	};

	m_pRigid->AddForce(m_Direction);
}

void Grenade::Update(const SceneContext& sceneContext)
{
	bool flagForDeletion{ false };

	m_Timer += sceneContext.pGameTime->GetElapsed();
	if (m_Timer > m_ExplosionDelay)
	{
		SetOnTriggerCallBack(m_Func);
		if (m_Timer > m_ExplosionDelay + m_ExplosionDuration)
		{
			flagForDeletion = true;
		}
	}
	if (flagForDeletion)
	{
		SceneManager::Get()->GetActiveScene()->RemoveChild(this, true);
	}
}

void Grenade::OnHit(GameObject* pTriggerObject, GameObject* pOtherObject, PxTriggerAction action)
{
	if (action == PxTriggerAction::ENTER)
	{
		std::wstring otherObjectTag = pOtherObject->GetTag();
		if (pTriggerObject->GetTag() != otherObjectTag)
		{
			Character* pCharacter = dynamic_cast<Character*>(pOtherObject);

			if (pCharacter != nullptr)
			{
				pCharacter->GetHit(m_Damage);
				return;
			}

			RobotEnemy* pEnemy = dynamic_cast<RobotEnemy*>(pOtherObject);

			if (pEnemy != nullptr)
			{
				pEnemy->GetHit(m_Damage);
				return;
			}
			
		}
	}
}
