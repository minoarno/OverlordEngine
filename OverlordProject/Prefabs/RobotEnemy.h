#pragma once
class Character;
class ColorMaterial_Shadow_Skinned;
class RobotEnemy : public GameObject
{
public:
	RobotEnemy();
	~RobotEnemy() override = default;

	RobotEnemy(const RobotEnemy& other) = delete;
	RobotEnemy(RobotEnemy&& other) noexcept = delete;
	RobotEnemy& operator=(const RobotEnemy& other) = delete;
	RobotEnemy& operator=(RobotEnemy&& other) noexcept = delete;

	void SetCharacter(Character* pCharacter) { m_pCharacter = pCharacter; };

	void SetPositions(const XMFLOAT3& pos1, const XMFLOAT3& pos2);

	void Reset();
protected:
	void Initialize(const SceneContext& sceneContext) override;
	void Update(const SceneContext& sceneContext) override;

	ColorMaterial_Shadow_Skinned* m_pEyeMat{ nullptr };
	GameObject* m_pVisuals{ nullptr };
	RigidBodyComponent* m_pRigidBody{ nullptr };
	Character* m_pCharacter{ nullptr };

	ParticleEmitterComponent* m_pEmitter{ nullptr };

	XMFLOAT3 m_Position1, m_Position2, m_Target;
	float m_SquaredTriggerDistance{ 250 };
	float m_MoveSpeed{ 10.f };
};

