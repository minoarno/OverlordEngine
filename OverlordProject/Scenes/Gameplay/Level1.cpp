#include "stdafx.h"
#include "Level1.h"

//Materials
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow_Skinned.h"

//Prefabs
#include "Prefabs/RobotEnemy.h"
#include "Prefabs/Character.h"

Level1::Level1()
	: GameScene{L"Level 1"}
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

	//Visuals
	//**************
	const auto pMaterial0 = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>(); //Shadow variant
	pMaterial0->SetDiffuseTexture(L"Textures/Head_c.png");

	const auto pMaterial1 = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>(); //Shadow variant
	pMaterial1->SetDiffuseTexture(L"Textures/Eye_c.png");

	const auto pMaterial2 = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>(); //Shadow variant
	pMaterial2->SetDiffuseTexture(L"Textures/Tex2_c.png");

	const auto pMaterial3 = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>(); //Shadow variant
	pMaterial3->SetDiffuseTexture(L"Textures/Tex1_c.png");

	const auto pObject = AddChild(new GameObject);
	const auto pModel = pObject->AddComponent(new ModelComponent(L"Meshes/Ratchet.ovm"));
	pModel->SetMaterial(pMaterial0, 0);
	pModel->SetMaterial(pMaterial1, 1);
	pModel->SetMaterial(pMaterial2, 2);
	pModel->SetMaterial(pMaterial3, 3);
	pModel->GetTransform()->Rotate(0, 180, 0);

	float scale = 0.01f;
	pObject->GetTransform()->Scale(scale, scale, scale);

	pAnimator = pModel->GetAnimator();
	pAnimator->SetAnimation(m_AnimationClipId);
	pAnimator->SetAnimationSpeed(m_AnimationSpeed);

	//Gather Clip Names
	m_ClipCount = pAnimator->GetClipCount();
	m_ClipNames = new char* [m_ClipCount];
	for (UINT i{ 0 }; i < m_ClipCount; ++i)
	{
		auto clipName = StringUtil::utf8_encode(pAnimator->GetClip(static_cast<int>(i)).name);
		const auto clipSize = clipName.size();
		m_ClipNames[i] = new char[clipSize + 1];
		strncpy_s(m_ClipNames[i], clipSize + 1, clipName.c_str(), clipSize);
	}

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

}

void Level1::Update()
{
}

void Level1::Draw()
{
}

void Level1::OnGUI()
{
	if (ImGui::CollapsingHeader("Visuals"))
	{
		if (ImGui::Button(pAnimator->IsPlaying() ? "PAUSE" : "PLAY"))
		{
			if (pAnimator->IsPlaying())pAnimator->Pause();
			else pAnimator->Play();
		}

		if (ImGui::Button("RESET"))
		{
			pAnimator->Reset();
		}

		ImGui::Dummy({ 0,5 });

		bool reversed = pAnimator->IsReversed();
		if (ImGui::Checkbox("Play Reversed", &reversed))
		{
			pAnimator->SetPlayReversed(reversed);
		}

		if (ImGui::ListBox("Animation Clip", &m_AnimationClipId, m_ClipNames, static_cast<int>(m_ClipCount)))
		{
			pAnimator->SetAnimation(m_AnimationClipId);
		}

		if (ImGui::SliderFloat("Animation Speed", &m_AnimationSpeed, 0.f, 4.f))
		{
			pAnimator->SetAnimationSpeed(m_AnimationSpeed);
		}
	}
}
