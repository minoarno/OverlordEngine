#include "stdafx.h"

#include "SpriteTestScene.h"

#include "Managers/ContentManager.h"
#include "Components/SpriteComponent.h"
#include "Components/TransformComponent.h"
#include "Base/Logger.h"

#define FPS_COUNTER 1

SpriteTestScene::SpriteTestScene() :
GameScene(L"SpriteTestScene"),
m_FpsInterval(FPS_COUNTER),
m_pObj(nullptr),
m_Counter(105.0f)
{}

void SpriteTestScene::Initialize()
{
	m_pObj = new GameObject();
	m_pObj->AddComponent(new SpriteComponent(L"Textures/TestSprite.jpg", DirectX::XMFLOAT2(0.5f, 0.5f), DirectX::XMFLOAT4(1, 1, 1, 0.5f)));
	AddChild(m_pObj);

	m_pObj->GetTransform()->Translate(500.f, 350.f, .9f);
	m_pObj->GetTransform()->Scale(1.f, 1.f, 1.f);
}

void SpriteTestScene::Update()
{

	m_FpsInterval += m_SceneContext.pGameTime->GetElapsed();
	if (m_FpsInterval >= FPS_COUNTER)
	{
		m_FpsInterval -= FPS_COUNTER;
		Logger::LogInfo(L"FPS: %i", m_SceneContext.pGameTime->GetFPS());
	}

	//m_pObj->GetTransform()->Rotate(0, 0, DirectX::XM_PIDIV2 * m_SceneContext.pGameTime->GetTotal(), false);
}

void SpriteTestScene::Draw()
{
}

void SpriteTestScene::OnGUI()
{
}
