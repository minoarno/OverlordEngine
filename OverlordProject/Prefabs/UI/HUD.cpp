#include "stdafx.h"
#include "HUD.h"
#include "Prefabs/UI/Text.h"

HUD* HUD::m_pHUD{ nullptr };

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

	//Hearts
	for (size_t i = 0; i < m_AmountOfHearts; i++)
	{
		m_pHearts.emplace_back(AddChild(new GameObject{}));
		m_pNutBolt = m_pHearts[i]->AddComponent(new SpriteComponent{L"Textures/Heart.png"});
		m_pHearts[i]->GetTransform()->Translate(45.f + 50 * i, 18.f, 0.f);
		m_pHearts[i]->GetTransform()->Scale(.05f, .05f, 0.5f);
	}

	if (m_pHUD == nullptr)
	{
		m_pHUD = this;
	}
}

void HUD::Draw(const SceneContext& sceneContext)
{
	m_pBoltsText->Draw(sceneContext);
}

void HUD::SetAmountOfHearts(int amount)
{
	m_AmountOfHearts = amount;

	for (size_t i = 0; i < m_pHearts.size(); i++)
	{
		m_pHearts[i]->SetActive(m_AmountOfHearts > i);
	}
}

void HUD::SetAmountBolts(int amount)
{
	m_AmountOfBoltsCollected = amount;
	UpdateBoltsText();
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