#pragma once
class Character;
class Crate : public GameObject
{
public:
	Crate();
	~Crate() override = default;

	Crate(const Crate& other) = delete;
	Crate(Crate&& other) noexcept = delete;
	Crate& operator=(const Crate& other) = delete;
	Crate& operator=(Crate&& other) noexcept = delete;

	void AttemptToBreak(Character* pCharacter);
	bool GetFlagToDelete()const { return m_FlagForDelete; }
protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;


private:
	bool m_FlagForDelete{ false };
	bool m_HasSpawnedReward{ false };
	float m_BreakDistanceSquared{ 36.f };
};

