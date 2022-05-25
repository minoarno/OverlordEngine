#pragma once
class Text : public GameObject
{
public:
	Text(const std::string& text, const XMFLOAT4& color = { 1.f,1.f,1.f,1.f });
	~Text() override = default;

	Text(const Text& other) = delete;
	Text(Text&& other) noexcept = delete;
	Text& operator=(const Text& other) = delete;
	Text& operator=(Text&& other) noexcept = delete;

	void Initialize(const SceneContext&) override;
	void Draw(const SceneContext&) override;

	void SetTextColor(const XMFLOAT4& color) { m_TextColor = color; }
	void SetText(const std::string& text) { m_Text = text; }
private:
	std::string m_Text{ "" };
	XMFLOAT4 m_TextColor{ 1.f,1.f,1.f,1.f };
	SpriteFont* m_pFont{ nullptr };
};

