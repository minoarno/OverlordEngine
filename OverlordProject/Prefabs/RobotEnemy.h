#pragma once

struct EnemyDesc
{
	EnemyDesc(
		PxMaterial* pMaterial,
		float radius = 1.5f,
		float height = 2.4f)
	{
		controller.setToDefault();
		controller.radius = radius;
		controller.height = height;
		controller.material = pMaterial;
	}

	float maxMoveSpeed{ 25.f };
	float maxFallSpeed{ 15.f };

	float moveAccelerationTime{ .3f };
	float fallAccelerationTime{ .3f };

	PxCapsuleControllerDesc controller{};

	float rotationSpeed{ 120.f };
};


class Character;
class ColorMaterial_Shadow_Skinned;
class RobotEnemy : public GameObject
{
public:
	enum EnemyAnimation : uint8_t
	{
		Dying = 0,
		Running = 1,
		Shooting = 2,
		Slashing = 3,
		GettingHit = 4
	};

	RobotEnemy();
	~RobotEnemy() override = default;

	RobotEnemy(const RobotEnemy& other) = delete;
	RobotEnemy(RobotEnemy&& other) noexcept = delete;
	RobotEnemy& operator=(const RobotEnemy& other) = delete;
	RobotEnemy& operator=(RobotEnemy&& other) noexcept = delete;

	void SetCharacter(Character* pCharacter) { m_pCharacter = pCharacter; };

	void SetPositions(const XMFLOAT3& pos1, const XMFLOAT3& pos2);

	void Reset();

	void GetHit();
protected:
	void Initialize(const SceneContext& sceneContext) override;
	void Update(const SceneContext& sceneContext) override;

	ColorMaterial_Shadow_Skinned* m_pEyeMat{ nullptr };
	GameObject* m_pVisuals{ nullptr };
	
	EnemyDesc m_EnemyDesc;
	ControllerComponent* m_pController{ nullptr };
	
	Character* m_pCharacter{ nullptr };

	EnemyAnimation m_EnemyAnimation{ EnemyAnimation::Running };
	ModelAnimator* m_pAnimator{ nullptr };

	ParticleEmitterComponent* m_pEmitter{ nullptr };

	XMFLOAT3 m_Position1, m_Position2, m_Target;
	float m_SquaredTriggerDistance{ 250 };
	float m_MoveSpeed{ 1.f };
	float m_FallSpeed{ 15.f };

	float m_TimerAttack{ 0.f };
	float m_DurationAttack{ 1.f };

	float m_TimerHit{ 0.f };
	float m_DurationHit{ 0.5f };

	int m_Health{ 3 };
};

