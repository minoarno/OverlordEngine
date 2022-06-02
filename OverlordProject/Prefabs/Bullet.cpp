#include "stdafx.h"
#include "Bullet.h"

#include "Character.h"
#include "RobotEnemy.h"

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

	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	m_pRigid->AddCollider(PxBoxGeometry{ 1.f,1.f,1.f }, *pDefaultMaterial, true);

	SetOnTriggerCallBack([=](GameObject* pTriggerObject, GameObject* pOtherObject, PxTriggerAction action)
		{
			Character* pCharacter = dynamic_cast<Character*>(pOtherObject);
			RobotEnemy* pEnemy = dynamic_cast<RobotEnemy*>(pOtherObject);
			if (action == PxTriggerAction::ENTER)
			{
				std::wstring otherObjectTag = pOtherObject->GetTag();
				if (pTriggerObject->GetTag() != otherObjectTag)
				{
					if (otherObjectTag == L"Friendly")
					{

						if (pCharacter != nullptr)
						{
							pCharacter->GetHit();

							return;
						}
					}

					if (otherObjectTag == L"Enemy")
					{
						if (pEnemy != nullptr)
						{
							pEnemy->GetHit();
							return;
						}
					}
				}
			}
		});
}

void Bullet::Update(const SceneContext& sceneContext)
{
	auto pos = m_pRigid->GetPosition();
	XMFLOAT3 newPos{};
	XMStoreFloat3(&newPos, XMVectorAdd(XMLoadFloat3(&pos), XMLoadFloat3(&m_Direction) * m_Speed * sceneContext.pGameTime->GetElapsed()));
	m_pRigid->Translate(newPos);
}

void Bullet::OnHit(GameObject* pTriggerObject, GameObject* pOtherObject, PxTriggerAction action)
{
	if (action == PxTriggerAction::ENTER)
	{
		std::wstring otherObjectTag = pOtherObject->GetTag();
		if (pTriggerObject->GetTag() != otherObjectTag)
		{
			if (otherObjectTag == L"Friendly")
			{
				Character* pCharacter = dynamic_cast<Character*>(pOtherObject);
				if (pCharacter != nullptr)
				{
					pCharacter->GetHit();

					return;
				}
			}

			if (otherObjectTag == L"Enemy")
			{
				RobotEnemy* pEnemy = dynamic_cast<RobotEnemy*>(pOtherObject);
				if (pEnemy != nullptr)
				{
					pEnemy->GetHit();
					return;
				}
			}	
		}
	}	
}


Bullet::~Bullet()
{
}
