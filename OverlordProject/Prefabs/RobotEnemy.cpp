#include "stdafx.h"
#include "RobotEnemy.h"

#include "Materials/Shadow/ColorMaterial_Shadow_Skinned.h"

#include "Prefabs/Character.h"

#include "Materials/ColorMaterial.h"

RobotEnemy::RobotEnemy()
	: m_Position1{ 0,0,0 }
	, m_Position2{ 0,0,0 }
	, m_Target{ 0,0,0 }
{
}

void RobotEnemy::SetPositions(const XMFLOAT3& pos1, const XMFLOAT3& pos2)
{
	m_Position1 = pos1;
	m_Position2 = pos2;
}

void RobotEnemy::Initialize(const SceneContext&)
{
	const auto pMat0 = MaterialManager::Get()->CreateMaterial<ColorMaterial_Shadow_Skinned>();
	pMat0->SetColor(DirectX::XMFLOAT4{ DirectX::Colors::Beige});

	const auto pMat1 = MaterialManager::Get()->CreateMaterial<ColorMaterial_Shadow_Skinned>();
	pMat1->SetColor(DirectX::XMFLOAT4{ DirectX::Colors::IndianRed });

	m_pEyeMat = MaterialManager::Get()->CreateMaterial<ColorMaterial_Shadow_Skinned>();
	m_pEyeMat->SetColor(DirectX::XMFLOAT4{ DirectX::Colors::Aquamarine });

	const auto pMat3 = MaterialManager::Get()->CreateMaterial<ColorMaterial_Shadow_Skinned>();
	pMat3->SetColor(DirectX::XMFLOAT4{ DirectX::Colors::DarkRed });

	//Character Mesh
	//**************
	m_pVisuals = AddChild(new GameObject);
	const auto pModel = m_pVisuals->AddComponent(new ModelComponent(L"Meshes/EnemyRobot.ovm"));
	pModel->SetMaterial(pMat0,0);
	pModel->SetMaterial(pMat1,1);
	pModel->SetMaterial(m_pEyeMat,2);
	pModel->SetMaterial(pMat3,3);

	float scale{ 0.01f };
	m_pVisuals->GetTransform()->Scale(scale, scale, scale);
	m_pVisuals->GetTransform()->Translate(0, -3.f, 0);

	if (const auto pAnimator = pModel->GetAnimator())
	{
		pAnimator->SetAnimation(1);
		pAnimator->Play();
	}

	const auto pDefaultMaterial = PxGetPhysics().createMaterial(1.0f, 1.0f, 0.5f);
	m_pRigidBody = AddComponent(new RigidBodyComponent());
	m_pRigidBody->AddCollider(PxBoxGeometry{ 2.f,3.f,2.f }, *pDefaultMaterial);
	m_pRigidBody->SetConstraint(RigidBodyConstraint::RotX | RigidBodyConstraint::RotZ, false);

	ParticleEmitterSettings settings{};
	settings.velocity = { 0.f,6.f,0.f };
	settings.minSize = 1.f;
	settings.maxSize = 2.f;
	settings.minEnergy = 1.f;
	settings.maxEnergy = 2.f;
	settings.minScale = 3.5f;
	settings.maxScale = 5.5f;
	settings.minEmitterRadius = .2f;
	settings.maxEmitterRadius = .5f;
	settings.color = { 1.f,1.f,1.f, .6f };
	m_pEmitter = AddComponent(new ParticleEmitterComponent(L"Textures/Smoke.png", settings, 200));

	//Tag
	SetTag(L"Enemy");
}

void RobotEnemy::Update(const SceneContext& sceneContext)
{

	if (m_pCharacter != nullptr && MathHelper::SquaredDistance(m_pCharacter->GetTransform()->GetPosition(), GetTransform()->GetPosition()) < m_SquaredTriggerDistance)
	{
		m_Target = m_pCharacter->GetTransform()->GetPosition();
		
		m_pEyeMat->SetColor(DirectX::XMFLOAT4{ DirectX::Colors::Red });
	}
	else
	{
		m_pEyeMat->SetColor(DirectX::XMFLOAT4{ DirectX::Colors::Aquamarine });
		DirectX::XMFLOAT3 pos = GetTransform()->GetPosition();
		float sqDist = MathHelper::SquaredDistance(m_Target, pos);
		if (sqDist < 100.f)
		{
			m_Target = (MathHelper::XMFloat3Equals(m_Target, m_Position1)) ? m_Position2 : m_Position2;
		}
	}

	XMFLOAT3 dir{};
	XMStoreFloat3(&dir, XMVectorSubtract(XMLoadFloat3(&m_Target), XMLoadFloat3(&GetTransform()->GetPosition())) * m_MoveSpeed * sceneContext.pGameTime->GetElapsed());

	m_pRigidBody->AddForce(dir);
}
