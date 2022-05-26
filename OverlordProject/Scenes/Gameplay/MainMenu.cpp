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

	m_pButtons.push_back(new Button(L"Textures/UI/StartButton.png", [&]() { SceneManager::Get()->SetActiveGameScene(L"Level 1"); }));
	m_pButtons[0]->GetTransform()->Translate(m_SceneContext.windowWidth * .25f, 200, .5f);
	AddChild(m_pButtons[0]);

	m_pButtons.push_back(new Button(L"Textures/UI/ExitButton.png", [&]() { return WM_QUIT; }));
	m_pButtons[1]->GetTransform()->Translate(m_SceneContext.windowWidth * .25f, 400, .5f);
	AddChild(m_pButtons[1]);

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
