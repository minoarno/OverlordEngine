#include "stdafx.h"
#include "RobotEnemy.h"

#include "Materials/Shadow/ColorMaterial_Shadow_Skinned.h"

#include "Prefabs/Character.h"

#include "Materials/ColorMaterial.h"

#include "BoltPicksUp.h"

RobotEnemy::RobotEnemy()
	: m_Position1{ 0,0,0 }
	, m_Position2{ 0,0,0 }
	, m_Target{ 0,0,0 }
	, m_EnemyDesc{ PxGetPhysics().createMaterial(0.5f, .0f, 0.5f)}
	, m_MusicVolume{ 0.3f }
{
}

void RobotEnemy::SetPositions(const XMFLOAT3& pos1, const XMFLOAT3& pos2)
{
	m_Position1 = pos1;
	m_Position2 = pos2;
	m_Target = m_Position1;
}

void RobotEnemy::Reset()
{
}

void RobotEnemy::GetHit(int damage)
{
	if (m_Timer == 0.f)
	{
		SoundManager::Get()->GetSystem()->playSound(m_pBackgroundSoundFx, m_pSoundEffectGroup, false, nullptr);
		m_pSoundEffectGroup->setVolume(m_MusicVolume);

		m_pEmitter->GetGameObject()->SetActive(true);
		m_EnemyAnimation = EnemyAnimation::GettingHit;
		m_Health -= damage;
		if (m_Health <= 0)
		{
			m_EnemyAnimation = EnemyAnimation::Dying;
		}

		m_pAnimator->SetAnimation(m_EnemyAnimation);
	}
}

bool RobotEnemy::GetFlagToDelete() const
{
	return m_FlagForDelete;
}

void RobotEnemy::Initialize(const SceneContext&)
{
	const auto pMat0 = MaterialManager::Get()->CreateMaterial<ColorMaterial_Shadow_Skinned>();
	pMat0->SetColor(DirectX::XMFLOAT4{ DirectX::Colors::Beige});

	const auto pMat1 = MaterialManager::Get()->CreateMaterial<ColorMaterial_Shadow_Skinned>();
	pMat1->SetColor(DirectX::XMFLOAT4{ DirectX::Colors::IndianRed });

	m_pEyeMat = MaterialManager::Get()->CreateMaterial<ColorMaterial_Shadow_Skinned>();
	m_pEyeMat->SetColor(DirectX::XMFLOAT4{ DirectX::Colors::Aquamarine });

	const auto pMat3 = MaterialManager::Get()->CreateMaterial<ColorMaterial_Shadow_Skinned>();
	pMat3->SetColor(DirectX::XMFLOAT4{ DirectX::Colors::DarkRed });

	//Character Mesh
	//**************
	m_pVisuals = AddChild(new GameObject);
	const auto pModel = m_pVisuals->AddComponent(new ModelComponent(L"Meshes/EnemyRobot.ovm"));
	pModel->SetMaterial(pMat0,0);
	pModel->SetMaterial(pMat1,1);
	pModel->SetMaterial(m_pEyeMat,2);
	pModel->SetMaterial(pMat3,3);

	float scale{ 0.01f };
	m_pVisuals->GetTransform()->Scale(scale, scale, scale);
	m_pVisuals->GetTransform()->Translate(0, -3.f, 0);

	m_pAnimator = pModel->GetAnimator();
	m_pAnimator->SetAnimation(EnemyAnimation::Running);
	m_pAnimator->Play();
	

	m_pController = AddComponent(new ControllerComponent(m_EnemyDesc.controller));

	ParticleEmitterSettings settings{};
	settings.minSize = 1.f;
	settings.maxSize = 2.f;
	settings.minEnergy = 1.f;
	settings.maxEnergy = 2.f;
	settings.minScale = 1.5f;
	settings.maxScale = 3.5f;
	settings.minEmitterRadius = .2f;
	settings.maxEmitterRadius = .8f;
	settings.color = { 1.f,1.f,1.f, .6f };
	settings.speed = 6.f;
	settings.useParticleEmitterVelocity = false;
	auto pEmitter = AddChild(new GameObject());
	m_pEmitter = pEmitter->AddComponent(new ParticleEmitterComponent(L"Textures/Sparks.png", settings, 200));
	pEmitter->SetActive(false);

	//auto
	auto fmodResult = SoundManager::Get()->GetSystem()->createChannelGroup("Sound Effects", &m_pSoundEffectGroup);
	SoundManager::Get()->ErrorCheck(fmodResult);

	SoundManager::Get()->GetSystem()->createStream("Resources/Audio/ReadyToFight.mp3", FMOD_DEFAULT, nullptr, &m_pBackgroundSoundFx);
	SoundManager::Get()->ErrorCheck(fmodResult);

	//Tag
	SetTag(L"Enemy");
}

void RobotEnemy::Update(const SceneContext& sceneContext)
{
	if (m_FlagForDelete)return;

	constexpr float epsilon{ 0.01f }; //Constant that can be used to compare if a float is near zero
	float elapsedTime = sceneContext.pGameTime->GetElapsed();

	if (m_EnemyAnimation == EnemyAnimation::Dying)
	{
		m_Timer += elapsedTime;
		if (m_Timer > m_DurationDying)
		{
			m_Timer = 0.f;
			m_pAnimator->SetAnimation(m_EnemyAnimation);

			auto activeScene = SceneManager::Get()->GetActiveScene();
			auto bolt = activeScene->AddChild(new BoltPicksUp{}, true);
			bolt->GetTransform()->Translate(GetTransform()->GetWorldPosition());

			m_FlagForDelete = true;
		}
		return;
		
	}
	else if (m_EnemyAnimation == EnemyAnimation::GettingHit)
	{
		m_Timer += elapsedTime;
		if (m_Timer > m_DurationHit)
		{
			m_Timer = 0.f;
			m_EnemyAnimation = EnemyAnimation::Running;
			m_pAnimator->SetAnimation(m_EnemyAnimation);
			m_pEmitter->Reset();
			m_pEmitter->GetGameObject()->SetActive(false);
		}
		else
		{
			return;
		}
	}
	else if (m_EnemyAnimation == EnemyAnimation::Slashing)
	{
		m_Timer += elapsedTime;
		if (m_Timer > m_DurationAttack)
		{
			m_Timer = 0.f;
			m_EnemyAnimation = EnemyAnimation::Running;
			m_pAnimator->SetAnimation(m_EnemyAnimation);
		}
		else
		{
			return;
		}
	}


	DirectX::XMFLOAT3 pos = GetTransform()->GetPosition();
	if (m_pCharacter != nullptr && MathHelper::SquaredDistance(m_pCharacter->GetTransform()->GetPosition(), GetTransform()->GetPosition()) < m_SquaredTriggerDistance)
	{
		m_Target = m_pCharacter->GetTransform()->GetPosition();
		
		m_pEyeMat->SetColor(DirectX::XMFLOAT4{ DirectX::Colors::Red });
		float sqDist = MathHelper::SquaredDistance(m_pCharacter->GetTransform()->GetPosition(), pos);
		if (sqDist < m_SquaredDestinationDistance)
		{
			if (m_Timer < epsilon)
			{
				m_EnemyAnimation = EnemyAnimation::Slashing;
				m_pAnimator->SetAnimation(m_EnemyAnimation);
				m_pCharacter->GetHit(m_Damage);
			}

			m_Timer += elapsedTime;
			if (m_Timer > m_DurationAttack)
			{
				m_Timer = 0.f;
			}
		}
	}
	else
	{
		m_pEyeMat->SetColor(DirectX::XMFLOAT4{ DirectX::Colors::Aquamarine });
		float sqDist = MathHelper::SquaredDistance(m_Target, pos);
		if (sqDist < 10.f)
		{
			m_Target = (MathHelper::XMFloat3Equals(m_Target, m_Position1)) ? m_Position2 : m_Position1;
		}
	}
	XMFLOAT3 dir{};
	float speed = elapsedTime * m_MoveSpeed;
	XMStoreFloat3(&dir, XMVectorSubtract(XMLoadFloat3(&m_Target), XMLoadFloat3(&pos)) * speed);

	if (m_pController->GetCollisionFlags().isSet(PxControllerCollisionFlag::eCOLLISION_DOWN))
	{
		dir.y = epsilon;
	}
	else
	{
		dir.y -= m_FallSpeed * elapsedTime;
		if (dir.y < -m_EnemyDesc.maxFallSpeed)
		{
			dir.y = -m_EnemyDesc.maxFallSpeed;
		}
	}

	m_pController->Move(dir,epsilon);

	XMStoreFloat3(&dir, XMVector3Normalize(XMLoadFloat3(&dir)));

	XMFLOAT3 forward{};
	XMStoreFloat3(&forward, XMVector3Normalize(XMLoadFloat3(&GetTransform()->GetForward())));

	float currentAngle = atan2(forward.x, forward.z);

	float desiredAngle = atan2(dir.x, dir.z);

	float look{desiredAngle - currentAngle };
	look *= 180 / XM_PI;

	if (look >= 180)
	{
		look -= 360;
	}
	else if(look <= 180)
	{
		look += 360;
	}

	if (abs(look ) > 10.f)
	{
		m_TotalYaw += look * m_EnemyDesc.rotationSpeed * elapsedTime;

		GetTransform()->Rotate(0, m_TotalYaw, 0, true);
	}
}
