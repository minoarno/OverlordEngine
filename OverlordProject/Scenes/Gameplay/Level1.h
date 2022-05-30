#pragma once
class HUD;
class Button;
class PostBloom;
class Character;
class FMOD::Sound;
class FMOD::ChannelGroup;
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


	void OnSceneActivated() override;
	void Reset();

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
	//Character
	Character* m_pCharacter{nullptr};

	//HUD and UI
	std::vector<Button*> m_pButtons;
	HUD* m_pHUD;

	//Audio
	FMOD::Sound* m_pBackgroundSoundFx;
	FMOD::ChannelGroup* m_pSoundEffectGroup;

	//Post Processing
	PostBloom* m_pPostBloom;
};

