#pragma once

class Grenade : public GameObject
{
public:
	Grenade(const XMFLOAT3& direction, const std::wstring& tag);
	~Grenade() override {};

	Grenade(const Grenade& other) = delete;
	Grenade(Grenade&& other) noexcept = delete;
	Grenade& operator=(const Grenade& other) = delete;
	Grenade& operator=(Grenade&& other) noexcept = delete;
protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;
private:
	float m_Speed{ 10.f };
	XMFLOAT3 m_Direction;
	RigidBodyComponent* m_pRigid{ nullptr };
};

