#pragma once
class HUD;

class Crate;
class Button;
class PostBloom;
class Character;
class RobotEnemy;
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
private:
	//Character
	Character* m_pCharacter{nullptr};
	XMFLOAT3 m_CharacterSpawn{};

	//Enemies
	std::vector<RobotEnemy*> m_pEnemies;
	std::vector<std::pair<XMFLOAT3, XMFLOAT3>> m_PositionsEnemy;

	//Crates
	std::vector<Crate*> m_pCrates;
	std::vector<XMFLOAT3> m_PositionsCrate;

	//HUD and UI
	std::vector<Button*> m_pButtons;
	GameObject* m_pVictoryScreen{ nullptr };
	GameObject* m_pLostScreen{ nullptr };

	//Audio
	FMOD::Sound* m_pBackgroundSoundFx;
	FMOD::ChannelGroup* m_pSoundEffectGroup;
	float m_MusicVolume{};

	//Post Processing
	PostBloom* m_pPostBloom;

	void RemoveEnemies();
	void SpawnEnemies();

	void RemoveCrates();
	void SpawnCrates();
};

