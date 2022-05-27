#pragma once

struct CharacterDesc
{
	CharacterDesc(
		PxMaterial* pMaterial,
		float radius = .5f,
		float height = 2.f)
	{
		controller.setToDefault();
		controller.radius = radius;
		controller.height = height;
		controller.material = pMaterial;
	}

	float maxMoveSpeed{ 25.f };
	float maxFallSpeed{ 15.f };

	float JumpSpeed{ 25.f };

	float moveAccelerationTime{ .3f };
	float fallAccelerationTime{ .3f };

	PxCapsuleControllerDesc controller{};

	float rotationSpeed{ 120.f };

	int actionId_MoveLeft{ -1 };
	int actionId_MoveRight{ -1 };
	int actionId_MoveForward{ -1 };
	int actionId_MoveBackward{ -1 };
	int actionId_Jump{ -1 };
};

class Character : public GameObject
{
public:
	enum CharacterAnimation : uint8_t
	{
		Idle = 0,
		Dying = 1,
		Shooting = 2,
		Slashing = 3,
		Throwing = 4,
		Running = 5,
		Jumping = 6
	};

	Character(const CharacterDesc& characterDesc);
	~Character() override;

	Character(const Character& other) = delete;
	Character(Character&& other) noexcept = delete;
	Character& operator=(const Character& other) = delete;
	Character& operator=(Character&& other) noexcept = delete;

	void DrawImGui();

protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;
private:
	CameraComponent* m_pCameraComponent{};
	ControllerComponent* m_pControllerComponent{};

	CharacterDesc m_CharacterDesc;
	float m_TotalPitch{0}, m_TotalYaw{0};				//Total camera Pitch(X) and Yaw(Y) rotation
	float m_MoveAcceleration{0},						//Acceleration required to reach maxMoveVelocity after 1 second (maxMoveVelocity / moveAccelerationTime)
		m_FallAcceleration{0},							//Acceleration required to reach maxFallVelocity after 1 second (maxFallVelocity / fallAccelerationTime)
		m_MoveSpeed{0};									//MoveSpeed > Horizontal Velocity = MoveDirection * MoveVelocity (= TotalVelocity.xz)

	XMFLOAT3 m_TotalVelocity{0,0,0};					//TotalVelocity with X/Z for Horizontal Movement AND Y for Vertical Movement (fall/jump)
	XMFLOAT3 m_CurrentDirection{0,0,0};					//Current/Last Direction based on Camera forward/right (Stored for deacceleration)

	//Visuals
	GameObject* m_pVisuals;
	ModelAnimator* pAnimator{};

	int m_AnimationClipId{ 0 };
	float m_AnimationSpeed{ 1.f };

	char** m_ClipNames{};
	UINT m_ClipCount{};

	CharacterAnimation m_pCharacterState{ CharacterAnimation::Idle };
};

