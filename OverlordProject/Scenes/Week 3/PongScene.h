#pragma once

class PongScene final : public GameScene
{
public:
	PongScene();
	~PongScene() override = default;

	PongScene(const PongScene& other) = delete;
	PongScene(PongScene&& other) noexcept = delete;
	PongScene& operator=(const PongScene& other) = delete;
	PongScene& operator=(PongScene&& other) noexcept = delete;
protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void OnGUI() override;
private:
	void Reset();

	FreeCamera* m_pFixedCamera{};

	GameObject* m_pBall = nullptr;
	GameObject* m_pPeddle1 = nullptr;
	GameObject* m_pPeddle2 = nullptr;
};

