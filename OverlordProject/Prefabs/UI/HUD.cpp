#include "stdafx.h"
#include "HUD.h"
#include "Prefabs/UI/Text.h"

HUD::HUD()
	: GameObject{ }
	, m_AmountOfBoltsCollected{ 0 }
	, m_pBoltsText{ nullptr }
{
}

void HUD::Initialize(const SceneContext&)
{
	float x{ 1000 };
	m_pBoltsText = AddChild(new Text{ "" });
	m_pBoltsText->GetTransform()->Translate(x, 20.f, 0.f);
	UpdateBoltsText();

	auto go = AddChild(new GameObject{});
	m_pNutBolt = go->AddComponent(new SpriteComponent{ L"Textures/NutBolt.png" });
	go->GetTransform()->Translate(x - 45.f, 18.f, 0.f);
	go->GetTransform()->Scale(.05f, .05f, 0.5f);
}

void HUD::Draw(const SceneContext& sceneContext)
{
	m_pBoltsText->Draw(sceneContext);
}

void HUD::SetAmountBolts(int amount)
{
	m_AmountOfBoltsCollected = amount;
}

void HUD::IncreaseBolts(int amount)
{
	m_AmountOfBoltsCollected += amount;
	UpdateBoltsText();
}

void HUD::UpdateBoltsText()
{
	m_pBoltsText->SetText(std::to_string(m_AmountOfBoltsCollected));
}