#include "stdafx.h"
#include "PongScene.h"

#include "Managers/InputManager.h"

#include "Prefabs/CubePrefab.h"
#include "Prefabs/SpherePrefab.h"

#include "Components/RigidBodyComponent.h"

PongScene::PongScene():	GameScene(L"PongScene") {}

void PongScene::Initialize()
{
	//m_SceneContext.settings.showInfoOverlay = true;
	//m_SceneContext.settings.drawPhysXDebug = true;
	//m_SceneContext.settings.drawGrid = true;
	m_SceneContext.settings.enableOnGUI = true;
	
	auto physX = PhysXManager::Get()->GetPhysics();
	auto pMaterial = physX->createMaterial(0, 0, 1);

	m_pBall = new SpherePrefab(1.f, 10, DirectX::XMFLOAT4{ DirectX::Colors::Red });
	m_pBall->GetTransform()->Translate(0, 0, 0);
	RigidBodyComponent* pRigidBodyBall = m_pBall->AddComponent(new RigidBodyComponent{});
	pRigidBodyBall->AddCollider(PxSphereGeometry{}, *pMaterial);
	pRigidBodyBall->SetKinematic(true);

	m_pPeddle1 = new CubePrefab(DirectX::XMFLOAT3{1,4,1}, DirectX::XMFLOAT4{DirectX::Colors::AliceBlue});
	m_pPeddle1->GetTransform()->Translate(-10,0,0);
	RigidBodyComponent* pRigidBodyPeddle1 = m_pPeddle1->AddComponent(new RigidBodyComponent{});
	pRigidBodyPeddle1->AddCollider(PxBoxGeometry{ .5f,2.f,.5f }, *pMaterial);
	pRigidBodyPeddle1->SetKinematic(true);

	m_pPeddle2 = new CubePrefab(DirectX::XMFLOAT3{1,4,1}, DirectX::XMFLOAT4{DirectX::Colors::PaleVioletRed});
	m_pPeddle2->GetTransform()->Translate(10, 0, 0);
	RigidBodyComponent* pRigidBodyPeddle2 = m_pPeddle2->AddComponent(new RigidBodyComponent{});
	pRigidBodyPeddle2->AddCollider(PxBoxGeometry{.5f,2.f,.5f}, *pMaterial);
	pRigidBodyPeddle2->SetKinematic(true);

}

void PongScene::Update()
{
	if (m_SceneContext.pInput->IsKeyboardKey(InputState::down, VK_UP))
	{
		auto pos = m_pPeddle1->GetTransform()->GetPosition();
		float movement = 20 * m_SceneContext.pGameTime->GetElapsed();
		pos.y += movement;
		m_pPeddle1->GetTransform()->Translate(pos);
	}
	if (m_SceneContext.pInput->IsKeyboardKey(InputState::down, VK_DOWN))
	{
		auto pos = m_pPeddle1->GetTransform()->GetPosition();
		float movement = 20 * m_SceneContext.pGameTime->GetElapsed();
		pos.y -= movement;
		m_pPeddle1->GetTransform()->Translate(pos);
	}

	if (m_SceneContext.pInput->IsKeyboardKey(InputState::down, 'W'))
	{
		auto pos = m_pPeddle2->GetTransform()->GetPosition();
		float movement = 20 * m_SceneContext.pGameTime->GetElapsed();
		pos.y += movement;
		m_pPeddle2->GetTransform()->Translate(pos);
	}
	if (m_SceneContext.pInput->IsKeyboardKey(InputState::down, 'S'))
	{
		auto pos = m_pPeddle2->GetTransform()->GetPosition();
		float movement = 20 * m_SceneContext.pGameTime->GetElapsed();
		pos.y -= movement;
		m_pPeddle2->GetTransform()->Translate(pos);
	}
	if (m_SceneContext.pInput->IsKeyboardKey(InputState::down, 'R'))
	{
	}
}

void PongScene::Draw()
{
}

void PongScene::OnGUI()
{
	ImGui::Text("This only activates if\n SceneSettings.enableOnGUI is True.\n\n");
	ImGui::Text("Use ImGui to add custom\n controllable scene parameters!");
	ImGui::ColorEdit3("Demo ClearColor", &m_SceneContext.settings.clearColor.x, ImGuiColorEditFlags_NoInputs);
}

void PongScene::Reset()
{
	m_pBall->GetTransform()->Translate(0, 0, 0);
	m_pPeddle1->GetTransform()->Translate(-10, 0, 0);
	m_pPeddle2->GetTransform()->Translate(10, 0, 0);
}
