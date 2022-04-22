#pragma once
#include "Scenegraph/GameScene.h"

class UberMaterialScene final : public GameScene
{
public:
	UberMaterialScene();
	virtual ~UberMaterialScene() = default;

	UberMaterialScene(const UberMaterialScene& other) = delete;
	UberMaterialScene(UberMaterialScene&& other) noexcept = delete;
	UberMaterialScene& operator=(const UberMaterialScene& other) = delete;
	UberMaterialScene& operator=(UberMaterialScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void OnGUI() override;
};

