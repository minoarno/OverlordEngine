#pragma once
#include "Utils/EffectHelper.h"
#include "Scenegraph/GameScene.h"
class SpikyScene final: public GameScene
{
public:
	SpikyScene();
	virtual ~SpikyScene() = default;

	SpikyScene(const SpikyScene& other) = delete;
	SpikyScene(SpikyScene&& other) noexcept = delete;
	SpikyScene& operator=(const SpikyScene& other) = delete;
	SpikyScene& operator=(SpikyScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void OnGUI() override;
private:
	float m_FpsInterval;
};

