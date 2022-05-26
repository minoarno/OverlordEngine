#pragma once
class ColorMaterial_Shadow_Skinned;
class RobotEnemy : public GameObject
{
public:
	RobotEnemy();
	~RobotEnemy() override = default;

	RobotEnemy(const RobotEnemy& other) = delete;
	RobotEnemy(RobotEnemy&& other) noexcept = delete;
	RobotEnemy& operator=(const RobotEnemy& other) = delete;
	RobotEnemy& operator=(RobotEnemy&& other) noexcept = delete;

protected:
	void Initialize(const SceneContext& sceneContext) override;

private:
	ColorMaterial_Shadow_Skinned* m_pEyeMat;
};

