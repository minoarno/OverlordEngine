#pragma once

class Grenade : public GameObject
{
public:
	Grenade(const XMFLOAT3& direction);
	~Grenade() override {};

	Grenade(const Grenade& other) = delete;
	Grenade(Grenade&& other) noexcept = delete;
	Grenade& operator=(const Grenade& other) = delete;
	Grenade& operator=(Grenade&& other) noexcept = delete;

	RigidBodyComponent* GetRigid()const { return m_pRigid; }
protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;

	void OnHit(GameObject* pTriggerObject, GameObject* pOtherObject, PxTriggerAction action);
private:
	float m_Speed{ 10.f };
	XMFLOAT3 m_Direction;
	RigidBodyComponent* m_pRigid{ nullptr };

	PhysicsCallback m_Func;

	float m_ExplosionRadius{10.f};

	float m_Timer{0.f};
	float m_ExplosionDelay{ 3.f };
	float m_ExplosionDuration{ 1.5f };
	int m_Damage{ 3 };
};

