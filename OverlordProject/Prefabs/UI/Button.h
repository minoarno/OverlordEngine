#pragma once
#include <functional>
class Text;
class Button : public GameObject
{
public:
	Button(const std::wstring& imgPathNormal, const std::wstring& imgPathActivated, const std::function<void()>& func);
	~Button() override = default;

	Button(const Button& other) = delete;
	Button(Button&& other) noexcept = delete;
	Button& operator=(const Button& other) = delete;
	Button& operator=(Button&& other) noexcept = delete;

	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;

	void Select();
	void Press(const SceneContext& sceneContext);
	bool IsSelected()const { return m_IsSelected; }
	bool IsHovering(const SceneContext& sceneContext);

private:
	bool m_IsSelected{ false };
	SpriteComponent* m_pSpriteComponentNormal{};
	SpriteComponent* m_pSpriteComponentActivated{};
	std::function<void()> m_Func;
};

