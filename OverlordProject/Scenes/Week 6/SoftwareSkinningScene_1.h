#pragma once

class BoneObject;

class SoftwareSkinningScene_2 final: public GameScene
{
public:
	SoftwareSkinningScene_2() : GameScene(L"SoftwareSkinningScene_1"){};
	~SoftwareSkinningScene_2() override = default;
	SoftwareSkinningScene_2(const SoftwareSkinningScene_2& other) = delete;
	SoftwareSkinningScene_2(SoftwareSkinningScene_2&& other) noexcept = delete;
	SoftwareSkinningScene_2& operator=(const SoftwareSkinningScene_2& other) = delete;
	SoftwareSkinningScene_2& operator=(SoftwareSkinningScene_2&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void OnGUI() override;

private:
	BoneObject* m_pBone0{}, * m_pBone1{};
	float m_BoneRotation{};
	int m_RotationSign{ 1 };
};
