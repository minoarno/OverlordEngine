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
	m_SceneContext.settings.enableOnGUI = false;

	//Camera
	const auto pCamera = AddChild(new FixedCamera());
	auto m_pCameraComponent = pCamera->GetComponent<CameraComponent>();
	m_pCameraComponent->SetActive(true);

	int index = 0;
	m_pButtons.push_back(new Button(L"Textures/UI/StartButton.png", [&]() { SceneManager::Get()->SetActiveGameScene(L"Level 1"); }));
	m_pButtons[index]->GetTransform()->Translate(m_SceneContext.windowWidth * .4f, 300, .5f);
	AddChild(m_pButtons[index]);

	index = 1;
	m_pButtons.push_back(new Button(L"Textures/UI/ExitButton.png", [&]() { OverlordGame::Stop(); }));
	m_pButtons[index]->GetTransform()->Translate(m_SceneContext.windowWidth * .4f, 500, .5f);
	AddChild(m_pButtons[index]);

	//Background
	auto background = new GameObject();
	background->AddComponent(new SpriteComponent(L"Textures/MainMenuBackground.png", DirectX::XMFLOAT2(0.5f, 0.5f), DirectX::XMFLOAT4(1, 1, 1, 0.5f)));
	background->GetTransform()->Translate(m_SceneContext.windowWidth * .5f, m_SceneContext.windowHeight * .5f, .9f);
	background->GetTransform()->Scale(1.f, 1.f, 1.f);

	AddChild(background);
}

void MainMenu::Update()
{
	if (InputManager::IsMouseButton(InputState::pressed, VK_LBUTTON))
	{
		for (Button* button : m_pButtons)
		{
			button->Press(m_SceneContext);
		}
	}
}

void MainMenu::OnGUI()
{
}
