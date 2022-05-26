#include "stdafx.h"
#include "Button.h"
#include "Prefabs/UI/Text.h"

Button::Button(const std::wstring& imgPath, const std::function<void()>& func)
	: GameObject{ }
	, m_Func{ func }
{
	AddComponent(new SpriteComponent(imgPath));

}


void Button::Initialize(const SceneContext&)
{
	const auto pMaterial = PxGetPhysics().createMaterial(.5f, .5f, .5f);
	auto pRigidBody = AddComponent(new RigidBodyComponent(true));
	pRigidBody->AddCollider(PxBoxGeometry{ 15.f,5.f,1.f }, *pMaterial);
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
