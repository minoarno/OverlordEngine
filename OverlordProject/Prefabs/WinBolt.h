#pragma once
class WinBolt : public GameObject
{
public:
	WinBolt();
	~WinBolt() override = default;

	WinBolt(const WinBolt& other) = delete;
	WinBolt(WinBolt&& other) noexcept = delete;
	WinBolt& operator=(const WinBolt& other) = delete;
	WinBolt& operator=(WinBolt&& other) noexcept = delete;

protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;

	void OnHit(GameObject* pTriggerObject, GameObject* pOtherObject, PxTriggerAction action);
private:
	RigidBodyComponent* m_pRigid{ nullptr };
};

