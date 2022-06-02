#pragma once
class Bullet : public GameObject
{
public:
	Bullet(const XMFLOAT3& direction, const std::wstring& tag);
	~Bullet() override;

	Bullet(const Bullet& other) = delete;
	Bullet(Bullet&& other) noexcept = delete;
	Bullet& operator=(const Bullet& other) = delete;
	Bullet& operator=(Bullet&& other) noexcept = delete;

protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;

private:
	float m_Speed{ 10.f };
	XMFLOAT3 m_Direction;
	RigidBodyComponent* m_pRigid{ nullptr };
};

