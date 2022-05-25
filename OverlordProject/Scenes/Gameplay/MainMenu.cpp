#include "stdafx.h"
#include "MainMenu.h"

#include "Prefabs/UI/Button.h"

MainMenu::MainMenu()
	:GameScene{ L"MainMenu" }
{
}

MainMenu::~MainMenu()
{
}

void MainMenu::Initialize()
{
	//Camera
	const auto pCamera = AddChild(new FixedCamera());
	auto m_pCameraComponent = pCamera->GetComponent<CameraComponent>();
	m_pCameraComponent->SetActive(true);

	auto startButton = new Button("Start", [&]() { SceneManager::Get()->NextScene(); });
	startButton->GetTransform()->Translate(200, 50, .5f);
	AddChild(startButton);

	//Background
	auto background = new GameObject();
	background->AddComponent(new SpriteComponent(L"Textures/MainMenuBackground.png", DirectX::XMFLOAT2(0.5f, 0.5f), DirectX::XMFLOAT4(1, 1, 1, 0.5f)));
	background->GetTransform()->Translate(m_SceneContext.windowWidth * .5f, m_SceneContext.windowHeight * .5f, .9f);
	background->GetTransform()->Scale(1.f, 1.f, 1.f);

	AddChild(background);


}

void MainMenu::Update()
{
	if (InputManager::IsMouseButton(InputState::pressed, VK_RBUTTON))
	{
		if (const auto pPickedObject = m_SceneContext.pCamera->Pick())
		{
			RemoveChild(pPickedObject, true);
		}
	}
}

void MainMenu::OnGUI()
{
}
