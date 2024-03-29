#include "stdafx.h"
#include "Character.h"
#include "PhysX/PhysxProxy.h"
#include "Scenegraph/GameScene.h"

#include "Materials/Shadow/DiffuseMaterial_Shadow_Skinned.h"

#include "Prefabs/Bullet.h"
#include "Prefabs/Grenade.h"

#include "UI/HUD.h"

Character::Character(const CharacterDesc& characterDesc)
	: m_CharacterDesc{ characterDesc }
	, m_MoveAcceleration(characterDesc.maxMoveSpeed / characterDesc.moveAccelerationTime)
	, m_FallAcceleration(characterDesc.maxFallSpeed / characterDesc.fallAccelerationTime)
	, m_pVisuals{ nullptr }
{}

Character::~Character()
{
}

void Character::Initialize(const SceneContext& /*sceneContext*/)
{
	//Controller
	m_pControllerComponent = AddComponent(new ControllerComponent(m_CharacterDesc.controller));

	//Camera
	GameObject* pCamera = AddChild(new FixedCamera());
	m_pCameraComponent = pCamera->GetComponent<CameraComponent>();
	m_pCameraComponent->SetActive(true); //Uncomment to make this camera the active camera

	pCamera->GetTransform()->Translate(0.f, m_CharacterDesc.controller.height * 1.5f, 15.f);
	pCamera->GetTransform()->Rotate(0, 180, 0);

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

	m_pVisuals = AddChild(new GameObject());
	const auto pModel = m_pVisuals->AddComponent(new ModelComponent(L"Meshes/Ratchet.ovm"));
	pModel->SetMaterial(pMaterial0, 0);
	pModel->SetMaterial(pMaterial1, 1);
	pModel->SetMaterial(pMaterial2, 2);
	pModel->SetMaterial(pMaterial3, 3);
	//pModel->GetTransform()->Rotate(0, 0, 0);

	float scale = 0.01f;
	m_pVisuals->GetTransform()->Scale(scale, scale, scale);
	m_pVisuals->GetTransform()->Translate(0, -m_CharacterDesc.controller.height * 1.1f, 0);

	m_pAnimator = pModel->GetAnimator();
	m_CharacterState = CharacterAnimation::Idle;
	m_pAnimator->SetAnimation(m_CharacterState);
	m_pAnimator->SetAnimationSpeed(m_AnimationSpeed);
	m_pAnimator->Play();

	SetTag(L"Friendly");
}

void Character::Update(const SceneContext& sceneContext)
{
	if (!sceneContext.pGameTime->IsRunning())return;

	if (m_CharacterState == CharacterAnimation::Dying || GetTransform()->GetPosition().y < -120)
	{
		m_AnimationTimer += sceneContext.pGameTime->GetElapsed();
		if (m_AnimationTimer > m_AnimationDuration)
		{
			m_pAnimator->Pause();
			m_DyingAnimationIsDone = true;

		}
		return;
	}

	if (m_pCameraComponent->IsActive())
	{
		constexpr float epsilon{ 0.01f }; //Constant that can be used to compare if a float is near zero

		//***************
		//HANDLE INPUT

		//## Input Gathering (move)
		XMFLOAT2 move = {0,0}; //Uncomment
		//move.y should contain a 1 (Forward) or -1 (Backward) based on the active input (check corresponding actionId in m_CharacterDesc)
		//Optional: if move.y is near zero (abs(move.y) < epsilon), you could use the ThumbStickPosition.y for movement

		if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveForward)) //forward
		{
			move.y = 1;
		}
		else if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveBackward)) //backward
		{
			move.y = -1;
		}
		if (abs(move.y) < epsilon)
		{
			move.y = sceneContext.pInput->GetThumbstickPosition().y;
		}

		//move.x should contain a 1 (Right) or -1 (Left) based on the active input (check corresponding actionId in m_CharacterDesc)
		//Optional: if move.x is near zero (abs(move.x) < epsilon), you could use the Left ThumbStickPosition.x for movement
		if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveRight)) //right
		{
			move.x = 1;
		}
		else if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveLeft)) //left
		{
			move.x = -1;
		}
		if (abs(move.x) < epsilon)
		{
			move.x = sceneContext.pInput->GetThumbstickPosition().x;
		}
		
		//## Input Gathering (look)
		XMFLOAT2 look{ 0.f, 0.f }; //Uncomment
		//Only if the Left Mouse Button is Down >
		// Store the MouseMovement in the local 'look' variable (cast is required)
		//Optional: in case look.x AND look.y are near zero, you could use the Right ThumbStickPosition for look
		auto mouseMovement = sceneContext.pInput->GetMouseMovement();
		if (sceneContext.pInput->IsMouseButton(InputState::down,0))
		{
			look = DirectX::XMFLOAT2{ (float)mouseMovement.x, (float)mouseMovement.y };
		}
		if (abs(look.x) < epsilon && abs(look.y) < epsilon)
		{
			look = sceneContext.pInput->GetThumbstickPosition(false);
		}

		//************************
		//GATHERING TRANSFORM INFO

		//Retrieve the TransformComponent
		//Retrieve the forward & right vector (as XMVECTOR) from the TransformComponent
		XMVECTOR forward = DirectX::XMLoadFloat3(&m_pCameraComponent->GetGameObject()->GetTransform()->GetForward()) * move.y;
		XMVECTOR right = DirectX::XMLoadFloat3(&m_pCameraComponent->GetGameObject()->GetTransform()->GetRight()) * move.x;

		//***************
		//CAMERA ROTATION

		//Adjust the TotalYaw (m_TotalYaw) & TotalPitch (m_TotalPitch) based on the local 'look' variable
		//Make sure this calculated on a framerate independent way and uses CharacterDesc::rotationSpeed.
		//Rotate this character based on the TotalPitch (X) and TotalYaw (Y)

		float elapsedTime  = sceneContext.pGameTime->GetElapsed();
		m_TotalYaw += look.x * m_CharacterDesc.rotationSpeed * elapsedTime;
		m_TotalPitch -= look.y * m_CharacterDesc.rotationSpeed * elapsedTime;

		//if (m_TotalPitch > m_MaxPitch)
		//{
		//	m_TotalPitch = m_MaxPitch;
		//}
		//else if (m_TotalPitch < m_MinPitch)
		//{
		//	m_TotalPitch = m_MinPitch;
		//}

		GetTransform()->Rotate(m_TotalPitch, m_TotalYaw, 0);
		//Clamp()
		m_pVisuals->GetTransform()->Rotate(-m_TotalPitch, 0, 0);

		//********
		//MOVEMENT


		//## Horizontal Velocity (Forward/Backward/Right/Left)
		//Calculate the current move acceleration for this frame (m_MoveAcceleration * ElapsedTime)
		//If the character is moving (= input is pressed)
			//Calculate & Store the current direction (m_CurrentDirection) >> based on the forward/right vectors and the pressed input
			//Increase the current MoveSpeed with the current Acceleration (m_MoveSpeed)
			//Make sure the current MoveSpeed stays below the maximum MoveSpeed (CharacterDesc::maxMoveSpeed)
		//Else (character is not moving, or stopped moving)
			//Decrease the current MoveSpeed with the current Acceleration (m_MoveSpeed)
			//Make sure the current MoveSpeed doesn't get smaller than zero
		m_MoveAcceleration += m_MoveAcceleration * elapsedTime;
		if (abs(move.x) > epsilon || abs(move.y) > epsilon)
		{
			XMStoreFloat3(&m_CurrentDirection, (forward + right));

			m_MoveSpeed += m_MoveAcceleration * elapsedTime;
			if (m_MoveSpeed > m_CharacterDesc.maxMoveSpeed)
			{
				m_MoveSpeed = m_CharacterDesc.maxMoveSpeed;
			}
		}
		else
		{
			m_MoveSpeed -= m_MoveAcceleration * elapsedTime;
			if (m_MoveSpeed < epsilon)
			{
				m_MoveSpeed = 0;
			}
		}

		//Now we can calculate the Horizontal Velocity which should be stored in m_TotalVelocity.xz
		//Calculate the horizontal velocity (m_CurrentDirection * MoveSpeed)
		//Set the x/z component of m_TotalVelocity (horizontal_velocity x/z)
		//It's important that you don't overwrite the y component of m_TotalVelocity (contains the vertical velocity)
		m_TotalVelocity.x = m_CurrentDirection.x * m_MoveSpeed;
		m_TotalVelocity.z = m_CurrentDirection.z * m_MoveSpeed;

		//## Vertical Movement (Jump/Fall)
		//If the Controller Component is NOT grounded (= freefall)
			//Decrease the y component of m_TotalVelocity with a fraction (ElapsedTime) of the Fall Acceleration (m_FallAcceleration)
			//Make sure that the minimum speed stays above -CharacterDesc::maxFallSpeed (negative!)
		//Else If the jump action is triggered
			//Set m_TotalVelocity.y equal to CharacterDesc::JumpSpeed
		//Else (=Character is grounded, no input pressed)
			//m_TotalVelocity.y is zero
		if (!m_pControllerComponent->GetCollisionFlags().isSet(PxControllerCollisionFlag::eCOLLISION_DOWN))
		{
			m_TotalVelocity.y -= m_FallAcceleration * elapsedTime;
			if (m_TotalVelocity.y < -m_CharacterDesc.maxFallSpeed)
			{
				m_TotalVelocity.y = -m_CharacterDesc.maxFallSpeed;
			}
		}
		else if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_Jump))
		{
			m_TotalVelocity.y = m_CharacterDesc.JumpSpeed;
		}
		else
		{
			m_TotalVelocity.y = 0;
		}

		//************
		//DISPLACEMENT

		//The displacement required to move the Character Controller (ControllerComponent::Move) can be calculated using our TotalVelocity (m/s)
		//Calculate the displacement (m) for the current frame and move the ControllerComponent

		//The above is a simple implementation of Movement Dynamics, adjust the code to further improve the movement logic and behaviour.
		//Also, it can be usefull to use a seperate RayCast to check if the character is grounded (more responsive)
		
		DirectX::XMFLOAT3 displacementFloat3;
		DirectX::XMStoreFloat3(&displacementFloat3, DirectX::XMLoadFloat3(&m_TotalVelocity)* sceneContext.pGameTime->GetElapsed());
		m_pControllerComponent->Move(displacementFloat3, epsilon);

		if (m_CharacterState != CharacterAnimation::Throwing && m_CharacterState != CharacterAnimation::Slashing && m_CharacterState != CharacterAnimation::Shooting)
		{
			if (abs(m_TotalVelocity.x) < epsilon && abs(m_TotalVelocity.y) < 1 && abs(m_TotalVelocity.z) < epsilon)
			{
				if (m_CharacterState != CharacterAnimation::Idle)
				{
					m_CharacterState = CharacterAnimation::Idle;
					m_pAnimator->SetAnimation(m_CharacterState);
				}
			}
			else if (m_TotalVelocity.y > 1)
			{
				if (m_CharacterState != CharacterAnimation::Jumping)
				{
					m_CharacterState = CharacterAnimation::Jumping;
					m_pAnimator->SetAnimation(m_CharacterState);
				}
			}
			else if (m_TotalVelocity.y < -1)
			{
				if (m_CharacterState != CharacterAnimation::Falling)
				{
					m_CharacterState = CharacterAnimation::Falling;
					m_pAnimator->SetAnimation(m_CharacterState);
				}
			}
			else
			{
				if (m_CharacterState != CharacterAnimation::Running)
				{
					m_CharacterState = CharacterAnimation::Running;
					m_pAnimator->SetAnimation(m_CharacterState);
				}
			}
		}
		else
		{
			m_AnimationTimer += elapsedTime;
			if (m_AnimationTimer > m_AnimationDuration)
			{
				if (m_CharacterState == CharacterAnimation::Slashing) m_SwingingAxe = false;

				m_pAnimator->SetAnimation(CharacterAnimation::Idle);
				m_CharacterState = CharacterAnimation::Idle;
				
			}
		}
		

		if (sceneContext.pInput->IsActionTriggered(InputIds::CharacterGrenade))
		{
			if (m_CharacterState != CharacterAnimation::Throwing && m_CharacterState != CharacterAnimation::Slashing && m_CharacterState != CharacterAnimation::Shooting)
			{
				m_CharacterState = CharacterAnimation::Throwing;
				m_pAnimator->SetAnimation(m_CharacterState);
				m_AnimationTimer = 0;
				m_AnimationDuration = 1.f;
				ThrowGrenade();
			}
		}
		if (sceneContext.pInput->IsActionTriggered(InputIds::CharacterSlash))
		{
			if (m_CharacterState != CharacterAnimation::Throwing && m_CharacterState != CharacterAnimation::Slashing && m_CharacterState != CharacterAnimation::Shooting)
			{
				m_CharacterState = CharacterAnimation::Slashing;
				m_pAnimator->SetAnimation(m_CharacterState);
				m_AnimationTimer = 0;
				m_AnimationDuration = 1.f;
				SlapAxe();
			}
		}
		if (sceneContext.pInput->IsActionTriggered(InputIds::CharacterShoot))
		{
			if (m_CharacterState != CharacterAnimation::Throwing && m_CharacterState != CharacterAnimation::Slashing && m_CharacterState != CharacterAnimation::Shooting)
			{
				m_CharacterState = CharacterAnimation::Shooting;
				m_pAnimator->SetAnimation(m_CharacterState);
				m_AnimationTimer = 0;
				m_AnimationDuration = .5f;
				Shoot();
			}
		}


	}
}

void Character::ThrowGrenade()
{
	XMFLOAT3 forward{};
	XMStoreFloat3(&forward, XMLoadFloat3(&GetTransform()->GetForward()) * -1);
	forward.y += .1f;
	XMStoreFloat3(&forward, XMVector3Normalize(XMLoadFloat3(&forward)));
	XMStoreFloat3(&forward, XMLoadFloat3(&forward) * 5);
	auto activeScene = SceneManager::Get()->GetActiveScene();
	Grenade* grenade = activeScene->AddChild(new Grenade{ forward },true);
	grenade->SetTag(GetTag());
	auto pos = GetTransform()->GetPosition();
	XMFLOAT3 bulletPos{};
	XMStoreFloat3(&bulletPos, XMVectorAdd(XMLoadFloat3(&pos), XMLoadFloat3(&forward)));
	grenade->GetTransform()->Translate(bulletPos);
}

void Character::Shoot()
{
	XMFLOAT3 forward{};
	XMStoreFloat3(&forward, XMLoadFloat3(&GetTransform()->GetForward()) * -1);
	auto activeScene = SceneManager::Get()->GetActiveScene();
	auto bullet = activeScene->AddChild(new Bullet{ forward},true);
	bullet->SetTag(GetTag());
	auto pos = GetTransform()->GetPosition();
 	XMFLOAT3 bulletPos{};
	XMStoreFloat3(&bulletPos, XMVectorAdd(XMLoadFloat3(&pos), XMLoadFloat3(&forward) * m_CharacterDesc.controller.radius * 1.2f));
	bullet->GetTransform()->Translate(bulletPos);
}

void Character::SlapAxe()
{
	m_SwingingAxe = true;
}

void Character::DrawImGui()
{
	if (ImGui::CollapsingHeader("Character"))
	{
		ImGui::Text(std::format("Move Speed: {:0.1f} m/s", m_MoveSpeed).c_str());
		ImGui::Text(std::format("Fall Speed: {:0.1f} m/s", m_TotalVelocity.y).c_str());

		ImGui::Text(std::format("Move Acceleration: {:0.1f} m/s2", m_MoveAcceleration).c_str());
		ImGui::Text(std::format("Fall Acceleration: {:0.1f} m/s2", m_FallAcceleration).c_str());

		const float jumpMaxTime = m_CharacterDesc.JumpSpeed / m_FallAcceleration;
		const float jumpMaxHeight = (m_CharacterDesc.JumpSpeed * jumpMaxTime) - (0.5f * (m_FallAcceleration * powf(jumpMaxTime, 2)));
		ImGui::Text(std::format("Jump Height: {:0.1f} m", jumpMaxHeight).c_str());

		ImGui::Dummy({ 0.f,5.f });
		if (ImGui::DragFloat("Max Move Speed (m/s)", &m_CharacterDesc.maxMoveSpeed, 0.1f, 0.f, 0.f, "%.1f") ||
			ImGui::DragFloat("Move Acceleration Time (s)", &m_CharacterDesc.moveAccelerationTime, 0.1f, 0.f, 0.f, "%.1f"))
		{
			m_MoveAcceleration = m_CharacterDesc.maxMoveSpeed / m_CharacterDesc.moveAccelerationTime;
		}

		ImGui::Dummy({ 0.f,5.f });
		if (ImGui::DragFloat("Max Fall Speed (m/s)", &m_CharacterDesc.maxFallSpeed, 0.1f, 0.f, 0.f, "%.1f") ||
			ImGui::DragFloat("Fall Acceleration Time (s)", &m_CharacterDesc.fallAccelerationTime, 0.1f, 0.f, 0.f, "%.1f"))
		{
			m_FallAcceleration = m_CharacterDesc.maxFallSpeed / m_CharacterDesc.fallAccelerationTime;
		}

		ImGui::Dummy({ 0.f,5.f });
		ImGui::DragFloat("Jump Speed", &m_CharacterDesc.JumpSpeed, 0.1f, 0.f, 0.f, "%.1f");
		ImGui::DragFloat("Rotation Speed (deg/s)", &m_CharacterDesc.rotationSpeed, 0.1f, 0.f, 0.f, "%.1f");

		bool isActive = m_pCameraComponent->IsActive();
		if(ImGui::Checkbox("Character Camera", &isActive))
		{
			m_pCameraComponent->SetActive(isActive);
		}
	}
}

void Character::GetHit(int damage)
{
	if (m_CharacterState != CharacterAnimation::Dying)
	{
		m_Health -= damage;
		HUD::Get()->SetAmountOfHearts(m_Health);

		if (m_Health <= 0)
		{
			m_CharacterState = CharacterAnimation::Dying;
			m_pAnimator->SetAnimation(m_CharacterState);
			m_AnimationTimer = 0;
			m_AnimationDuration = m_DyingAnimatioonDuration;
		}
	}
}

void Character::Reset()
{
	m_TotalVelocity = XMFLOAT3{};
	m_TotalPitch = 0.f;
	m_TotalYaw = 0.f;
	m_Health = m_MaxHealth;
	HUD::Get()->SetAmountOfHearts(m_Health);

	m_CharacterState = CharacterAnimation::Idle;
	m_AnimationTimer = 0;
	m_pAnimator->SetAnimation(m_CharacterState);
	m_pAnimator->Play();

	m_DyingAnimationIsDone = false;
	m_SwingingAxe = false;
}
