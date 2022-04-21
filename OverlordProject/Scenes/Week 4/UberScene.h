#pragma once
class UberScene final: public GameScene
{
public:
	UberScene(std::wstring sceneName = L"UberScene");
	UberScene(const UberScene& other) = delete;
	UberScene(UberScene&& other) noexcept = delete;
	UberScene& operator=(const UberScene& other) = delete;
	UberScene& operator=(UberScene&& other) noexcept = delete;
	virtual ~UberScene();
protected:
	virtual void Initialize() {};
	virtual void Update() {};
	virtual void Draw() {};
	virtual void OnGUI() {};
};

