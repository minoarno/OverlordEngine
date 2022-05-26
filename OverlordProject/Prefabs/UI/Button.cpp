#include "stdafx.h"
#include "Button.h"
#include "Prefabs/UI/Text.h"

Button::Button(const std::wstring& imgPath, const std::function<void()>& func)
	: GameObject{ }
	, m_Func{ func }
{
	m_pSpriteComponent = AddComponent(new SpriteComponent(imgPath));
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

void Button::Press(const SceneContext& sceneContext)
{
	auto mousePos = sceneContext.pInput->GetMousePosition();
	auto pos = GetTransform()->GetPosition();
	auto dims = m_pSpriteComponent->GetDimensions();
	if (pos.x < mousePos.x && pos.x + dims.x > mousePos.x && pos.y < mousePos.y && pos.y + dims.y > mousePos.y)
	{
		m_Func();
	}
}
