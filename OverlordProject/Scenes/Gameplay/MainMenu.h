#pragma once
class MainMenu : public GameScene
{
public:
	MainMenu();
	~MainMenu();

	MainMenu(const MainMenu& other) = delete;
	MainMenu(MainMenu&& other) noexcept = delete;
	MainMenu& operator=(const MainMenu& other) = delete;
	MainMenu& operator=(MainMenu&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	//void Draw() override;
	void OnGUI() override;

};

