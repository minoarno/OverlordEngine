#pragma once
class RangedRobotEnemy : public RobotEnemy
{
public:
public:
	RangedRobotEnemy();
	~RangedRobotEnemy() override = default;

	RangedRobotEnemy(const RangedRobotEnemy& other) = delete;
	RangedRobotEnemy(RangedRobotEnemy&& other) noexcept = delete;
	RangedRobotEnemy& operator=(const RangedRobotEnemy& other) = delete;
	RangedRobotEnemy& operator=(RangedRobotEnemy&& other) noexcept = delete;

protected:
	void Initialize(const SceneContext& sceneContext) override;
	void Update(const SceneContext& sceneContext) override;
private:
};

