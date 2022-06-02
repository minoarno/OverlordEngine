#pragma once
class Text;
class HUD : public GameObject
{
public:
	HUD();
	~HUD() override = default;

	HUD(const HUD& other) = delete;
	HUD(HUD&& other) noexcept = delete;
	HUD& operator=(const HUD& other) = delete;
	HUD& operator=(HUD&& other) noexcept = delete;

	void Initialize(const SceneContext&) override;
	void Draw(const SceneContext&) override;

	void SetAmountBolts(int amount);
	void IncreaseBolts(int amount);
	static HUD* Get() { return m_pHUD; };
private:
	static HUD* m_pHUD;
	int m_AmountOfBoltsCollected{ 0 };
	SpriteComponent* m_pNutBolt{nullptr};
	Text* m_pBoltsText{nullptr};
	void UpdateBoltsText();
};

