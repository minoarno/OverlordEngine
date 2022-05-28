#include "stdafx.h"
#include "Level1.h"

//Materials
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow_Skinned.h"

//Prefabs
#include "Prefabs/RobotEnemy.h"
#include "Prefabs/Character.h"

#include "Prefabs/UI/HUD.h"
#include "Prefabs/UI/Button.h"

Level1::Level1()
	: GameScene{L"Level 1"}
{

}

Level1::~Level1()
{
}

void Level1::Initialize()
{
	m_SceneContext.settings.enableOnGUI = true;

	const auto pGroundMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>(); //Shadow variant
	pGroundMaterial->SetDiffuseTexture(L"Textures/GroundDirt.png");

	//Ground Plane
	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	GameSceneExt::CreatePhysXGroundPlane(*this, pDefaultMaterial);

	//Character
	CharacterDesc characterDesc{ pDefaultMaterial };
	characterDesc.actionId_MoveForward = CharacterMoveForward;
	characterDesc.actionId_MoveBackward = CharacterMoveBackward;
	characterDesc.actionId_MoveLeft = CharacterMoveLeft;
	characterDesc.actionId_MoveRight = CharacterMoveRight;
	characterDesc.actionId_Jump = CharacterJump;

	float levelScale{ 4.f };

	m_pCharacter = AddChild(new Character(characterDesc));
	m_pCharacter->GetTransform()->Translate(0.f, 10.f * levelScale, -10.f);

	//Ground
	const auto pLevelObject = AddChild(new GameObject());
	const auto pLevelMesh = pLevelObject->AddComponent(new ModelComponent(L"Meshes/Ground1.ovm"));
	auto matGround = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	matGround->SetDiffuseTexture(L"Textures/GroundDirt.png");
	pLevelMesh->SetMaterial(matGround);

	const auto pLevelActor = pLevelObject->AddComponent(new RigidBodyComponent(true));
	const auto pPxTriangleMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/Ground1.ovpt");
	pLevelActor->AddCollider(PxTriangleMeshGeometry(pPxTriangleMesh, PxMeshScale({ levelScale,levelScale,levelScale })), *pDefaultMaterial);
	pLevelObject->GetTransform()->Scale(levelScale, levelScale, levelScale);
	pLevelObject->GetTransform()->Rotate(90.f, 0.f, 0.f);

	//Input
	auto inputAction = InputAction(CharacterMoveLeft, InputState::down, 'A');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterMoveRight, InputState::down, 'D');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterMoveForward, InputState::down, 'W');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterMoveBackward, InputState::down, 'S');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterJump, InputState::pressed, VK_SPACE, -1, XINPUT_GAMEPAD_A);
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(Settings, InputState::pressed, VK_ESCAPE, -1, XINPUT_GAMEPAD_START);
	m_SceneContext.pInput->AddInputAction(inputAction);

	//HUD
	m_pHUD = AddChild(new HUD{});

	//In Game Buttons
	int index = 0;
	m_pButtons.push_back(new Button(L"Textures/UI/MainMenuButton.png", [&]() { SceneManager::Get()->SetActiveGameScene(L"MainMenu"); }));
	m_pButtons[index]->GetTransform()->Translate(m_SceneContext.windowWidth * .35f, 150, .5f);
	m_pButtons[index]->SetActive(false);
	AddChild(m_pButtons[index]);

	index = 1;
	m_pButtons.push_back(new Button(L"Textures/UI/RestartButton.png", [&]() { Reset(); }));
	m_pButtons[index]->GetTransform()->Translate(m_SceneContext.windowWidth * .35f, 350, .5f);
	m_pButtons[index]->SetActive(false);
	AddChild(m_pButtons[index]);

	index = 2;
	m_pButtons.push_back(new Button(L"Textures/UI/ExitButton.png", [&]() { OverlordGame::Stop(); }));
	m_pButtons[index]->GetTransform()->Translate(m_SceneContext.windowWidth * .35f, 550, .5f);
	m_pButtons[index]->SetActive(false);
	AddChild(m_pButtons[index]);

	//Enemies
	auto enemy0 = AddChild(new RobotEnemy{});
	enemy0->GetTransform()->Translate(20, 30, 0);


}

void Level1::Update()
{
	if (m_SceneContext.pInput->IsActionTriggered(Settings))
	{
		if (m_SceneContext.pGameTime->IsRunning())
		{
			m_SceneContext.pGameTime->Stop();
			for (Button* button : m_pButtons)
			{
				button->SetActive(true);
			}
		}
		else
		{
			m_SceneContext.pGameTime->Start();
			for (Button* button : m_pButtons)
			{
				button->SetActive(false);
			}
		}
	}
	if (InputManager::IsMouseButton(InputState::pressed, VK_LBUTTON))
	{
		for (Button* button : m_pButtons)
		{
			button->Press(m_SceneContext);
		}
	}
}

void Level1::Draw()
{
}

void Level1::OnGUI()
{
	m_pCharacter->DrawImGui();
}

void Level1::OnSceneActivated()
{
	Reset();
}

void Level1::Reset()
{
	for (Button* button : m_pButtons)
	{
		button->SetActive(false);
	}

	m_pCharacter->GetTransform()->Translate(0.f, 30.f, -10.f);

	m_pHUD->SetAmountBolts(0);

	m_SceneContext.pGameTime->Start();
}
