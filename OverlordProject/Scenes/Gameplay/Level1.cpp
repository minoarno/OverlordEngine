#include "stdafx.h"
#include "Level1.h"

//Materials
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow_Skinned.h"

//Prefabs
#include "Prefabs/RobotEnemy.h"
#include "Prefabs/Character.h"

#include "Prefabs/UI/HUD.h"

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

	//Ground Mesh
	//***********
	const auto pGroundObj = new GameObject();
	const auto pGroundModel = new ModelComponent(L"Meshes/UnitPlane.ovm");
	pGroundModel->SetMaterial(pGroundMaterial);

	pGroundObj->AddComponent(pGroundModel);
	pGroundObj->GetTransform()->Scale(10.0f, 10.0f, 10.0f);

	AddChild(pGroundObj);

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

	//HUD
	AddChild(new HUD{});

	//Enemies
	auto enemy0 = AddChild(new RobotEnemy{});
	enemy0->GetTransform()->Translate(20, 30, 0);
}

void Level1::Update()
{
}

void Level1::Draw()
{
}

void Level1::OnGUI()
{
	
}
