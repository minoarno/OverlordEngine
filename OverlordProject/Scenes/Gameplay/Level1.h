#pragma once
class Character;
class Level1 : public GameScene
{
public:
	Level1();
	~Level1() override;

	Level1(const Level1& other) = delete;
	Level1(Level1&& other) noexcept = delete;
	Level1& operator=(const Level1& other) = delete;
	Level1& operator=(Level1&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void OnGUI() override;

	enum InputIds
	{
		CharacterMoveLeft,
		CharacterMoveRight,
		CharacterMoveForward,
		CharacterMoveBackward,
		CharacterJump,
		CharacterShoot,
		CharacterSlash,
		CharacterGrenade,
		Settings
	};
private:
	Character* m_pCharacter{};

};

