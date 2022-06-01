#include "stdafx.h"
#include "Level1.h"

//Materials
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow_Skinned.h"

//Prefabs
#include "Prefabs/RobotEnemy.h"
#include "Prefabs/Character.h"
#include "Prefabs/Skybox.h"

//UI
#include "Prefabs/UI/HUD.h"
#include "Prefabs/UI/Button.h"

//Audio
#include "Managers/SoundManager.h"

//Post Processing
#include "Materials/Post/PostBloom.h"

Level1::Level1()
	: GameScene{L"Level 1"}
	, m_pHUD{ nullptr }
	, m_pBackgroundSoundFx{ nullptr }
	, m_pSoundEffectGroup{ nullptr }
	, m_pPostBloom{ nullptr }
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
	GameObject* pLevelObject = AddChild(new GameObject());
	ModelComponent* pLevelMesh = pLevelObject->AddComponent(new ModelComponent(L"Meshes/Ground1.ovm"));
	DiffuseMaterial_Shadow* matGround = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	matGround->SetDiffuseTexture(L"Textures/GroundDirt.png");
	pLevelMesh->SetMaterial(matGround);

	RigidBodyComponent* pLevelActor = pLevelObject->AddComponent(new RigidBodyComponent(true));
	PxTriangleMesh* pPxTriangleMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/Ground1.ovpt");
	pLevelActor->AddCollider(PxTriangleMeshGeometry(pPxTriangleMesh, PxMeshScale({ levelScale,levelScale,levelScale })), *pDefaultMaterial);
	pLevelObject->GetTransform()->Scale(levelScale, levelScale, levelScale);
	pLevelObject->GetTransform()->Rotate(90.f, 0.f, 0.f);

	levelScale = 10.f;
	pLevelObject = AddChild(new GameObject());
	pLevelMesh = pLevelObject->AddComponent(new ModelComponent(L"Meshes/Ground2.ovm"));
	matGround = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	matGround->SetDiffuseTexture(L"Textures/GroundDirt.png");
	pLevelMesh->SetMaterial(matGround);

	pLevelActor = pLevelObject->AddComponent(new RigidBodyComponent(true));
	pPxTriangleMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/Ground2.ovpt");
	pLevelActor->AddCollider(PxTriangleMeshGeometry(pPxTriangleMesh, PxMeshScale({ levelScale,levelScale,levelScale })), *pDefaultMaterial);
	pLevelActor->Translate(XMFLOAT3{ 0,30, 100 });
	pLevelObject->GetTransform()->Scale(levelScale, levelScale, levelScale);
	pLevelObject->GetTransform()->Rotate(90.f, 0.f, 0.f);

	//Input
	InputAction inputAction = InputAction(CharacterMoveLeft, InputState::down, 'Q');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterMoveRight, InputState::down, 'D');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterMoveForward, InputState::down, 'Z');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterMoveBackward, InputState::down, 'S');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterJump, InputState::pressed, VK_SPACE, -1, XINPUT_GAMEPAD_A);
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterShoot, InputState::pressed, 'A', -1, XINPUT_GAMEPAD_RIGHT_SHOULDER);
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterGrenade, InputState::pressed, 'R', -1, XINPUT_GAMEPAD_X);
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterSlash, InputState::pressed, 'E', -1, XINPUT_GAMEPAD_B);
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(Settings, InputState::pressed, VK_ESCAPE, -1, XINPUT_GAMEPAD_START);
	m_SceneContext.pInput->AddInputAction(inputAction);

	//HUD
	m_pHUD = AddChild(new HUD{});

	//In Game Buttons
	int index = 0;
	m_pButtons.push_back(new Button(L"Textures/UI/MainMenuButtonNormal.png", L"Textures/UI/MainMenuButtonActivated.png", [&]() { SceneManager::Get()->SetActiveGameScene(L"MainMenu"); }));
	m_pButtons[index]->GetTransform()->Translate(m_SceneContext.windowWidth * .35f, 150, .5f);
	m_pButtons[index]->SetActive(false);
	AddChild(m_pButtons[index]);

	index = 1;
	m_pButtons.push_back(new Button(L"Textures/UI/RestartButtonNormal.png", L"Textures/UI/RestartButtonActivated.png", [&]() { Reset(); }));
	m_pButtons[index]->GetTransform()->Translate(m_SceneContext.windowWidth * .35f, 350, .5f);
	m_pButtons[index]->SetActive(false);
	AddChild(m_pButtons[index]);

	index = 2;
	m_pButtons.push_back(new Button(L"Textures/UI/ExitButtonNormal.png", L"Textures/UI/ExitButtonActivated.png", [&]() { OverlordGame::Stop(); }));
	m_pButtons[index]->GetTransform()->Translate(m_SceneContext.windowWidth * .35f, 550, .5f);
	m_pButtons[index]->SetActive(false);
	AddChild(m_pButtons[index]);

	//Enemies
	index = 0;
	m_pEnemies.emplace_back(AddChild(new RobotEnemy{}));
	m_pEnemies[index]->GetTransform()->Translate(0, 26, 0);
	m_pEnemies[index]->SetPositions(XMFLOAT3{ 20,26,0 }, XMFLOAT3{ 0,26,0 });
	m_pEnemies[index]->SetCharacter(m_pCharacter);

	//Audio
	auto pFmodSystem = SoundManager::Get()->GetSystem();

	//Channel group
	auto fmodResult = pFmodSystem->createChannelGroup("Sound Effects", &m_pSoundEffectGroup);
	SoundManager::Get()->ErrorCheck(fmodResult);

	//Background music
	m_MusicVolume = 0.f;

	pFmodSystem->createStream("Resources/Audio/ReadyToFight.mp3", FMOD_DEFAULT, nullptr, &m_pBackgroundSoundFx);
	SoundManager::Get()->ErrorCheck(fmodResult);

	pFmodSystem->playSound(m_pBackgroundSoundFx, m_pSoundEffectGroup, false, nullptr);
	m_pSoundEffectGroup->setVolume(m_MusicVolume);

	AddChild(new Skybox{});

	//Post Processing
	m_pPostBloom = MaterialManager::Get()->CreateMaterial<PostBloom>();
	AddPostProcessingEffect(m_pPostBloom);

	m_pPostBloom->SetThreshold(0.1f);
	m_pPostBloom->SetStrength(.8f);
}

void Level1::Update()
{
	if (m_SceneContext.pGameTime->IsRunning())
	{
		if (m_SceneContext.pInput->IsActionTriggered(Settings))
		{
			m_SceneContext.pGameTime->Stop();
			for (Button* button : m_pButtons)
			{
				button->SetActive(true);
			}
		}
	}
	else
	{
		if (m_SceneContext.pInput->IsActionTriggered(Settings))
		{
			m_SceneContext.pGameTime->Start();
			for (Button* button : m_pButtons)
			{
				button->SetActive(false);
			}
		}

		if (InputManager::IsMouseButton(InputState::pressed, VK_LBUTTON))
		{
			for (Button* button : m_pButtons)
			{
				button->Press(m_SceneContext);
			}
		}

		POINT mousePos = InputManager::GetMousePosition();
		POINT prevMousePos = InputManager::GetMousePosition(true);
		if (mousePos.x != prevMousePos.x || mousePos.y != prevMousePos.y)
		{
			for (Button* button : m_pButtons)
			{
				button->IsHovering(m_SceneContext);
			}
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

	//Character
	m_pCharacter->GetTransform()->Translate(0.f, 30.f, -10.f);
	m_pCharacter->GetTransform()->Rotate(0.f, 0.f, 0.f);

	//Enemies
	m_pEnemies[0]->GetTransform()->Translate(0, 26, 0);

	for (size_t i = 0; i < m_pEnemies.size(); i++)
	{
		m_pEnemies[i]->Reset();
	}

	//HUD
	m_pHUD->SetAmountBolts(0);

	auto pFmodSystem = SoundManager::Get()->GetSystem();
	pFmodSystem->recordStop(-1);
	pFmodSystem->playSound(m_pBackgroundSoundFx, m_pSoundEffectGroup, false, nullptr);
	m_pSoundEffectGroup->setVolume(m_MusicVolume);

	m_SceneContext.pGameTime->Start();
}
