#include "stdafx.h"

#include "SpikyScene.h"

#include "PhysX/PhysxProxy.h"
#include "Components/RigidBodyComponent.h"
#include "Components/ModelComponent.h"
#include "Materials/SpikeyMaterial.h"


#define FPS_COUNTER 1

SpikyScene::SpikyScene():
	GameScene(L"SpikeyScene"),
	m_FpsInterval(FPS_COUNTER)
{}

void SpikyScene::Initialize()
{
	GetPhysxProxy()->EnablePhysxDebugRendering(true);

	// Create PhysX ground plane
	auto physX = PhysXManager::Get()->GetPhysics();

	auto bouncyMaterial = physX->createMaterial(0, 0, 1);
	auto ground = new GameObject();
	auto rigidBody = ground->AddComponent(new RigidBodyComponent(true));

	rigidBody->AddCollider(physx::PxPlaneGeometry{}, *bouncyMaterial, false, physx::PxTransform(physx::PxQuat(DirectX::XM_PIDIV2, physx::PxVec3(0, 0, 1))));
	AddChild(ground);


	//Material Test
	auto myMaterial = MaterialManager::Get()->CreateMaterial<SpikeyMaterial>();
	myMaterial->SetDiffuseTexture(L"Textures/Chair_Dark.dds");

	
	auto model = new ModelComponent(L"Meshes/Teapot.ovm");
	model->SetMaterial(60);

	auto teapot = new GameObject();
	teapot->AddComponent(model);
	AddChild(teapot);
	teapot->GetTransform()->Translate(0,0,20);
}

void SpikyScene::Update()
{

	m_FpsInterval += m_SceneContext.pGameTime->GetElapsed();
	if (m_FpsInterval >= FPS_COUNTER)
	{
		m_FpsInterval -= FPS_COUNTER;
		Logger::LogInfo(L"FPS: %i", m_SceneContext.pGameTime->GetFPS());
	}
}

void SpikyScene::Draw()
{}

void SpikyScene::OnGUI()
{
}
