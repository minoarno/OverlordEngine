#include "stdafx.h"
#include "Grenade.h"

Grenade::Grenade(const XMFLOAT3& direction, const std::wstring& tag)
	: GameObject{}
	, m_Direction{ direction }
{
	SetTag(tag);
}

void Grenade::Initialize(const SceneContext&)
{
}

void Grenade::Update(const SceneContext&)
{
}
