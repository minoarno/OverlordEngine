#pragma once
class BoltPicksUp final : public GameObject
{
public:
	BoltPicksUp();
	~BoltPicksUp() override = default;

	BoltPicksUp(const BoltPicksUp& other) = delete;
	BoltPicksUp(BoltPicksUp&& other) noexcept = delete;
	BoltPicksUp& operator=(const BoltPicksUp& other) = delete;
	BoltPicksUp& operator=(BoltPicksUp&& other) noexcept = delete;

protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;

	void OnSceneDetach(GameScene* pScene) override;

	void OnHit(GameObject* pTriggerObject, GameObject* pOtherObject, PxTriggerAction action);
private:
	int m_Score{ 50 };
	float m_RotationSpeed{ 90.f };
	float m_Rotation{ 0.f };
	RigidBodyComponent* m_pRigid{ nullptr };

	bool m_FlagForDelete{ false };
};

