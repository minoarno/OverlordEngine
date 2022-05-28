#include "stdafx.h"
#include "RobotEnemy.h"

#include "Materials/Shadow/ColorMaterial_Shadow_Skinned.h"

RobotEnemy::RobotEnemy()
{
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

	m_pEyeMat->SetColor(DirectX::XMFLOAT4{ DirectX::Colors::Red });

	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	auto rb = AddComponent(new RigidBodyComponent());
	rb->AddCollider(PxBoxGeometry{ 2.f,3.f,2.f }, *pDefaultMaterial);
}
