#include "stdafx.h"
#include "Level1.h"

//Materials
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow_Skinned.h"

//Prefabs
#include "Prefabs/RobotEnemy.h"
#include "Prefabs/Crate.h"
#include "Prefabs/Character.h"
#include "Prefabs/Skybox.h"
#include "Prefabs/WinBolt.h"

//UI
#include "Prefabs/UI/HUD.h"
#include "Prefabs/UI/Button.h"

//Audio
#include "Managers/SoundManager.h"

//Post Processing
#include "Materials/Post/PostBloom.h"

Level1::Level1()
	: GameScene{L"Level 1"}
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
	m_SceneContext.settings.enableOnGUI = false;
	m_SceneContext.settings.drawPhysXDebug = false;
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.showInfoOverlay = false;

	const auto pGroundMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>(); //Shadow variant
	pGroundMaterial->SetDiffuseTexture(L"Textures/GroundDirt.png");

	//Ground Plane
	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	//GameSceneExt::CreatePhysXGroundPlane(*this, pDefaultMaterial);

	//Character
	CharacterDesc characterDesc{ pDefaultMaterial };
	characterDesc.actionId_MoveForward = CharacterMoveForward;
	characterDesc.actionId_MoveBackward = CharacterMoveBackward;
	characterDesc.actionId_MoveLeft = CharacterMoveLeft;
	characterDesc.actionId_MoveRight = CharacterMoveRight;
	characterDesc.actionId_Jump = CharacterJump;

	m_pCharacter = AddChild(new Character(characterDesc));
	m_CharacterSpawn = { 1.2f, 3.6f, 131.f };
	m_pCharacter->GetTransform()->Translate(m_CharacterSpawn);
	m_pCharacter->GetTransform()->Rotate(0.f, 180.f, 0.f);

	//Ground
	float levelScale{ 1.f };
	GameObject* pLevelObject = AddChild(new GameObject());
	ModelComponent* pLevelMesh = pLevelObject->AddComponent(new ModelComponent(L"Meshes/Level1.ovm"));
	DiffuseMaterial_Shadow* matGround = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	matGround->SetDiffuseTexture(L"Textures/GroundDirt.png");
	pLevelMesh->SetMaterial(matGround);

	RigidBodyComponent* pLevelActor = pLevelObject->AddComponent(new RigidBodyComponent(true));
	PxTriangleMesh* pPxTriangleMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/Level1.ovpt");
	pLevelActor->AddCollider(PxTriangleMeshGeometry(pPxTriangleMesh, PxMeshScale({ levelScale,levelScale,levelScale })), *pDefaultMaterial);
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
	AddChild(new HUD{});

	//In Game Buttons
	int index = 0;
	m_pButtons.push_back(new Button(L"Textures/UI/MainMenuButtonNormal.png", L"Textures/UI/MainMenuButtonActivated.png", [&]() { SceneManager::Get()->SetActiveGameScene(L"MainMenu"); }));
	m_pButtons[index]->GetTransform()->Translate(m_SceneContext.windowWidth * .35f, 200, .5f);
	m_pButtons[index]->SetActive(false);
	AddChild(m_pButtons[index]);

	index = 1;
	m_pButtons.push_back(new Button(L"Textures/UI/RestartButtonNormal.png", L"Textures/UI/RestartButtonActivated.png", [&]() { Reset(); }));
	m_pButtons[index]->GetTransform()->Translate(m_SceneContext.windowWidth * .35f, 350, .5f);
	m_pButtons[index]->SetActive(false);
	AddChild(m_pButtons[index]);

	index = 2;
	m_pButtons.push_back(new Button(L"Textures/UI/ExitButtonNormal.png", L"Textures/UI/ExitButtonActivated.png", [&]() { OverlordGame::Stop(); }));
	m_pButtons[index]->GetTransform()->Translate(m_SceneContext.windowWidth * .35f, 500, .5f);
	m_pButtons[index]->SetActive(false);
	AddChild(m_pButtons[index]);

	//Win and lose
	m_pVictoryScreen = new GameObject();
	m_pVictoryScreen->AddComponent(new SpriteComponent(L"Textures/VictoryScreen.png", DirectX::XMFLOAT2(0.5f, 0.5f), DirectX::XMFLOAT4(1, 1, 1, 0.5f)));
	m_pVictoryScreen->GetTransform()->Translate(m_SceneContext.windowWidth * .5f, m_SceneContext.windowHeight * .5f, .9f);
	m_pVictoryScreen->GetTransform()->Scale(1.f, 1.f, 1.f);
	AddChild(m_pVictoryScreen);

	m_pLostScreen = new GameObject();
	m_pLostScreen->AddComponent(new SpriteComponent(L"Textures/LostScreen.png", DirectX::XMFLOAT2(0.5f, 0.5f), DirectX::XMFLOAT4(1, 1, 1, 0.5f)));
	m_pLostScreen->GetTransform()->Translate(m_SceneContext.windowWidth * .5f, m_SceneContext.windowHeight * .5f, .9f);
	m_pLostScreen->GetTransform()->Scale(1.f, 1.f, 1.f);
	AddChild(m_pLostScreen);

	m_pVictoryScreen->SetActive(false);
	m_pLostScreen->SetActive(false);
	
	//Enemies
	//m_PositionsEnemy.emplace_back(std::make_pair(XMFLOAT3{ 64.f,3.6f,74.f }, XMFLOAT3{ 43.6f,3.6f,52.2f }));
	//m_PositionsEnemy.emplace_back(std::make_pair(XMFLOAT3{ 50.f,3.6f,-21.5f }, XMFLOAT3{ 63.f,3.6f, -33.5f }));
	//m_PositionsEnemy.emplace_back(std::make_pair(XMFLOAT3{ 97.5f,-42.f,-34.f }, XMFLOAT3{ 85.f, -42.f, -27.f }));
	//m_PositionsEnemy.emplace_back(std::make_pair(XMFLOAT3{ 121.f,-42.f,-24.f }, XMFLOAT3{ 120.f, -42.f, -38.f }));


	//Crates
	m_PositionsCrate.emplace_back(XMFLOAT3{ 15.f, 3.6f, 100.f });
	m_PositionsCrate.emplace_back(XMFLOAT3{ -18.f, 3.6f, 100.f });
	m_PositionsCrate.emplace_back(XMFLOAT3{ 52.f, 3.6f, 0.5f });
	m_PositionsCrate.emplace_back(XMFLOAT3{ 105.f, -42.f, -40.f });

	//Winbolt
	auto winbolt = AddChild(new WinBolt{});
	winbolt->GetTransform()->Translate(214.6f, -104.F, -30.f);

	//Audio
	auto fmodResult = SoundManager::Get()->GetSystem()->createChannelGroup("Sound Effects", &m_pSoundEffectGroup);
	SoundManager::Get()->ErrorCheck(fmodResult);

	m_MusicVolume = 0.f;

	SoundManager::Get()->GetSystem()->createStream("Resources/Audio/ReadyToFight.mp3", FMOD_DEFAULT, nullptr, &m_pBackgroundSoundFx);
	SoundManager::Get()->ErrorCheck(fmodResult);

	SoundManager::Get()->GetSystem()->playSound(m_pBackgroundSoundFx, m_pSoundEffectGroup, false, nullptr);
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
	if (m_pCharacter->DidIWin())
	{
		m_pPostBloom->SetIsEnabled(false);
		m_pVictoryScreen->SetActive(true);

		m_SceneContext.pGameTime->Stop();
		for (Button* button : m_pButtons)
		{
			button->SetActive(true);
		}

		if (InputManager::IsMouseButton(InputState::pressed, VK_LBUTTON))
		{
			for (Button* button : m_pButtons)
			{
				button->Press(m_SceneContext);
			}
		}
	}

	if (m_pCharacter->GetIsDyingAnimationDone())
	{
		m_pPostBloom->SetIsEnabled(true);
		m_pLostScreen->SetActive(true);

		m_SceneContext.pGameTime->Stop();
		for (Button* button : m_pButtons)
		{
			button->SetActive(true);
		}

		if (InputManager::IsMouseButton(InputState::pressed, VK_LBUTTON))
		{
			for (Button* button : m_pButtons)
			{
				button->Press(m_SceneContext);
			}
		}
	}

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

	//Clean up enemies
	for (size_t i = 0; i < m_pEnemies.size(); i++)
	{
		if (m_pEnemies[i] != nullptr && m_pEnemies[i]->GetFlagToDelete())
		{
			RemoveChild(m_pEnemies[i],true);
			m_pEnemies[i] = nullptr;
		}
	}

	//Attempt to break crates
	if (m_pCharacter != nullptr && m_pCharacter->GetIsSwingingAxe())
	{
		for (size_t i = 0; i < m_pCrates.size(); i++)
		{
			if (m_pCrates[i] != nullptr)
			{
				m_pCrates[i]->AttemptToBreak(m_pCharacter);
			}
		}
	}

	//Clean up crates
	for (size_t i = 0; i < m_pCrates.size(); i++)
	{
		if (m_pCrates[i] != nullptr && m_pCrates[i]->GetFlagToDelete())
		{
			RemoveChild(m_pCrates[i], true);
			m_pCrates[i] = nullptr;
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
	m_pPostBloom->SetIsEnabled(true);

	for (Button* button : m_pButtons)
	{
		button->SetActive(false);
	}

	//Character
	m_pCharacter->GetTransform()->Translate(m_CharacterSpawn);
	m_pCharacter->GetTransform()->Rotate(0.f, 180.f, 0.f);
	m_pCharacter->Reset();

	//Enemies
	RemoveEnemies();
	SpawnEnemies();

	//Crates
	RemoveCrates();
	SpawnCrates();

	//HUD
	HUD::Get()->SetAmountBolts(0);

	auto pFmodSystem = SoundManager::Get()->GetSystem();
	pFmodSystem->recordStop(-1);
	pFmodSystem->playSound(m_pBackgroundSoundFx, m_pSoundEffectGroup, false, nullptr);
	m_pSoundEffectGroup->setVolume(m_MusicVolume);

	m_SceneContext.pGameTime->Start();

	m_pVictoryScreen->SetActive(false);
	m_pLostScreen->SetActive(false);
}

void Level1::SpawnEnemies()
{
	for (size_t i = 0; i < m_PositionsEnemy.size(); i++)
	{
		m_pEnemies.emplace_back(AddChild(new RobotEnemy{}));
		m_pEnemies[i]->GetTransform()->Translate(m_PositionsEnemy[i].first);
		m_pEnemies[i]->SetPositions(m_PositionsEnemy[i].first, m_PositionsEnemy[i].second);
		m_pEnemies[i]->SetCharacter(m_pCharacter);
	}
}

void Level1::RemoveEnemies()
{
	for (size_t i = 0; i < m_pEnemies.size(); i++)
	{
		if ( m_pEnemies[i] != nullptr)
		{
			RemoveChild(m_pEnemies[i], true);
		}
	}
	m_pEnemies.clear();
}

void Level1::SpawnCrates()
{
	for (size_t i = 0; i < m_PositionsCrate.size(); i++)
	{
		m_pCrates.emplace_back(AddChild(new Crate{}));
		m_pCrates[i]->GetTransform()->Translate(m_PositionsCrate[i]);
	}
}

void Level1::RemoveCrates()
{
	for (size_t i = 0; i < m_pCrates.size(); i++)
	{
		if (m_pCrates[i] != nullptr)
		{
			RemoveChild(m_pCrates[i], true);
		}
	}
	m_pCrates.clear();
}