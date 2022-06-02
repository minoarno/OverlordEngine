#include "stdafx.h"
#include "Skybox.h"

#include "Materials/SkyboxMaterial.h"

Skybox::Skybox()
	: GameObject{ }
{
}

void Skybox::Initialize(const SceneContext&)
{
	float scale{ 1000.f };

	const auto pMat0 = MaterialManager::Get()->CreateMaterial<SkyboxMaterial>();
	pMat0->SetCubemapTexture(L"Textures/bkg1_bot.png");

	auto component = AddComponent(new ModelComponent(L"Meshes/Skybox.ovm",false));
	component->SetMaterial(pMat0);
	GetTransform()->Scale(scale, scale, scale);
}
