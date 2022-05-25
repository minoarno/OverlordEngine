#include "stdafx.h"
#include "Button.h"
#include "Prefabs/UI/Text.h"

Button::Button(const std::string& text, const std::function<void()>& func)
	: GameObject{ }
	, m_Func{ func }
{
	m_pText = AddChild(new Text{ text });
}


void Button::Initialize(const SceneContext&)
{
}

void Button::Update(const SceneContext&)
{
}

void Button::Select()
{
	m_IsSelected = !m_IsSelected;
}

void Button::Press()
{
	m_Func();
}
