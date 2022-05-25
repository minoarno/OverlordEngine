#include "stdafx.h"
#include "Text.h"


Text::Text(const std::string& text, const XMFLOAT4& color)
	: m_Text{ text }
	, m_TextColor{ color }
{
}

void Text::Initialize(const SceneContext&)
{
	m_pFont = ContentManager::Load<SpriteFont>(L"SpriteFonts/Consolas_32.fnt");
}

void Text::Draw(const SceneContext&)
{
	DirectX::XMFLOAT2 pos{ GetTransform()->GetWorldPosition().x ,GetTransform()->GetWorldPosition().y };
	TextRenderer::Get()->DrawText(m_pFont, StringUtil::utf8_decode(m_Text),pos , m_TextColor);
}
