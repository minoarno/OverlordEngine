#include "stdafx.h"
#include "Crate.h"

#include "Materials/Shadow/DiffuseMaterial_Shadow.h"

#include "Prefabs/Character.h"
#include "Prefabs/BoltPicksUp.h"

Crate::Crate()
	: GameObject{ }
	, m_FlagForDelete{ false }
{
}

void Crate::AttemptToBreak(Character* pCharacter)
{
	float sqDist = MathHelper::SquaredDistance(pCharacter->GetTransform()->GetPosition(), GetTransform()->GetPosition());
	if (sqDist < m_BreakDistanceSquared)
	{
		m_FlagForDelete = true;
		auto activeScene = SceneManager::Get()->GetActiveScene();
		auto bolt = activeScene->AddChild(new BoltPicksUp{}, true);
		bolt->GetTransform()->Translate(GetTransform()->GetWorldPosition());
	}
}

void Crate::Initialize(const SceneContext&)
{
	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	float scale{ 2.f };

	ModelComponent* visuals = AddComponent(new ModelComponent(L"Meshes/Crate.ovm"));
	DiffuseMaterial_Shadow* matGround = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	matGround->SetDiffuseTexture(L"Textures/Crate.png");
	visuals->SetMaterial(matGround);

	RigidBodyComponent* actor = AddComponent(new RigidBodyComponent());
	actor->AddCollider(PxBoxGeometry{ scale,scale,scale }, *pDefaultMaterial);
	GetTransform()->Scale(scale, scale, scale);
}

void Crate::Update(const SceneContext&)
{
	if (m_FlagForDelete && !m_HasSpawnedReward)
	{
		m_HasSpawnedReward = true;
	}
}
