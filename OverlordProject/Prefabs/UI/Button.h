#pragma once
#include <functional>
class Text;
class Button : public GameObject
{
public:
	Button(const std::string& text, const std::function<void()>& func);
	~Button() override = default;

	Button(const Button& other) = delete;
	Button(Button&& other) noexcept = delete;
	Button& operator=(const Button& other) = delete;
	Button& operator=(Button&& other) noexcept = delete;

	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;

	void Select();
	void Press();
	bool IsSelected()const { return m_IsSelected; }
private:
	Text* m_pText;
	bool m_IsSelected{ false };
	std::function<void()> m_Func;
};

