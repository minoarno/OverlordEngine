#include "stdafx.h"
#include "RobotEnemy.h"

#include "Materials/Shadow/DiffuseMaterial_Shadow_Skinned.h"

RobotEnemy::RobotEnemy()
{
}

void RobotEnemy::Initialize(const SceneContext&)
{
	const auto pMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>(); //Shadow variant
	pMat->SetDiffuseTexture(L"Textures/PeasantGirl_Diffuse.png");

	//Character Mesh
	//**************
	const auto pObject = AddChild(new GameObject);
	const auto pModel = pObject->AddComponent(new ModelComponent(L"Meshes/EnemyRobot.ovm"));
	pModel->SetMaterial(pMat,0);

	pObject->GetTransform()->Scale(0.1f, 0.1f, 0.1f);

	if (const auto pAnimator = pModel->GetAnimator())
	{
		pAnimator->SetAnimation(2);
		pAnimator->Play();
	}
}
