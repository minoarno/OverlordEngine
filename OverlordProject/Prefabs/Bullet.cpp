#include "stdafx.h"
#include "Bullet.h"

#include "Character.h"
#include "RobotEnemy.h"

#include "Materials/ColorMaterial.h"

#include <typeinfo>

Bullet::Bullet(const XMFLOAT3& direction)
	: GameObject{}
	, m_Direction{ direction }
{
}

void Bullet::Initialize(const SceneContext&)
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
	const auto pModel = visuals->AddComponent(new ModelComponent(L"Meshes/Bullet.ovm"));
	pModel->SetMaterial(pMat);
	visuals->GetTransform()->Scale(scale, scale, scale);

	SetOnTriggerCallBack([=](GameObject* pTriggerObject, GameObject* pOtherObject, PxTriggerAction action)
		{
			OnHit(pTriggerObject, pOtherObject, action);
		});
}

void Bullet::Update(const SceneContext& sceneContext)
{
	auto pos = m_pRigid->GetPosition();
	XMFLOAT3 newPos{};
	float elapsed = sceneContext.pGameTime->GetElapsed();
	XMStoreFloat3(&newPos, XMVectorAdd(XMLoadFloat3(&pos), XMLoadFloat3(&m_Direction) * m_Speed * elapsed));
	m_pRigid->Translate(newPos);

	m_Timer += elapsed;
	if (m_Timer > m_Duration)
	{
		m_FlagForDelete = true;
	}

	if (m_FlagForDelete)
	{
		SceneManager::Get()->GetActiveScene()->RemoveChild(this, true);
	}
}

void Bullet::OnHit(GameObject* pTriggerObject, GameObject* pOtherObject, PxTriggerAction action)
{
	if (action == PxTriggerAction::ENTER)
	{
		std::wstring otherObjectTag = pOtherObject->GetTag();
		if (pTriggerObject->GetTag() != otherObjectTag)
		{
			Character* pCharacter = dynamic_cast<Character*>(pOtherObject);
			if (otherObjectTag == L"Friendly")
			{

				if (pCharacter != nullptr)
				{
					pCharacter->GetHit(m_Damage);
					m_FlagForDelete = true;
					return;
				}
			}

			RobotEnemy* pEnemy = dynamic_cast<RobotEnemy*>(pOtherObject);
			if (otherObjectTag == L"Enemy")
			{
				if (pEnemy != nullptr)
				{
					pEnemy->GetHit(m_Damage);
					m_FlagForDelete = true;
					return;
				}
			}
		}
	}
}


Bullet::~Bullet()
{
}
