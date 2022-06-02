#include "stdafx.h"
#include "Bullet.h"

Bullet::Bullet(const XMFLOAT3& direction, const std::wstring& tag)
	: GameObject{}
	, m_Direction{ direction }
{
	SetTag(tag);
}

void Bullet::Initialize(const SceneContext&)
{
	m_pRigid = AddComponent(new RigidBodyComponent());
	m_pRigid->SetKinematic(true);

	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	m_pRigid->AddCollider(PxBoxGeometry{ 1.f,1.f,1.f }, *pDefaultMaterial, true);
	m_pRigid->AddForce(m_Direction);
}

void Bullet::Update(const SceneContext&)
{
}


Bullet::~Bullet()
{
}
