#include "stdafx.h"
#include "RobotEnemy.h"
#include "RangedRobotEnemy.h"

#include "Materials/Shadow/ColorMaterial_Shadow_Skinned.h"

#include "Prefabs/Character.h"

RangedRobotEnemy::RangedRobotEnemy()
{
}

void RangedRobotEnemy::Initialize(const SceneContext& sceneContext)
{
	RobotEnemy::Initialize(sceneContext);

}

void RangedRobotEnemy::Update(const SceneContext& sceneContext)
{
	if (MathHelper::SquaredDistance(m_pCharacter->GetTransform()->GetPosition(), GetTransform()->GetPosition()) < m_SquaredTriggerDistance)
	{
		m_Target = m_pCharacter->GetTransform()->GetPosition();

		m_pEyeMat->SetColor(DirectX::XMFLOAT4{ DirectX::Colors::Red });
	}
	else
	{
		if (MathHelper::SquaredDistance(m_Target, GetTransform()->GetPosition()) < 1.f)
		{
			m_Target = (MathHelper::XMFloat3Equals(m_Target, m_Position1)) ? m_Position2 : m_Position2;
		}
	}

	XMFLOAT3 dir{};
	XMStoreFloat3(&dir, XMVectorSubtract(XMLoadFloat3(&m_Target), XMLoadFloat3(&GetTransform()->GetPosition())) * m_MoveSpeed * sceneContext.pGameTime->GetElapsed());

	GetTransform()->Translate(dir);


}
