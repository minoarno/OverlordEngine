#include "stdafx.h"

#include "ModelTestScene.h"
#include "Components/MeshDrawComponent.h"
#include "Managers/ContentManager.h"
#include "Managers/PhysxManager.h"


#include "Managers/MaterialManager.h"
#include "Materials/ColorMaterial.h"
#include "Materials/DiffuseMaterial.h"

ModelTestScene::ModelTestScene() :
GameScene(L"ModelTestScene"),
m_pChair(nullptr)
{}

void ModelTestScene::Initialize()
{
	//GROUND PLANE
	//************
	auto physX = PhysXManager::Get()->GetPhysics();

#pragma warning(push)
#pragma warning(disable:4189)
	auto pBouncyMaterial = physX->createMaterial(0, 0, 1);
	auto pGround = new GameObject();
	auto pRigidBodyGround = pGround->AddComponent(new RigidBodyComponent(true));

	pRigidBodyGround->AddCollider(physx::PxPlaneGeometry{}, *pBouncyMaterial, false, physx::PxTransform(physx::PxQuat(DirectX::XM_PIDIV2, physx::PxVec3(0, 0, 1))));
	AddChild(pGround);
#pragma warning(pop)
	//CHAIR OBJECT
	//************
	m_pChair = new GameObject();

	//1. Attach a modelcomponent (chair.ovm)
	//2. Create a ColorMaterial and add it to the material manager
	//3. Assign the material to the previous created modelcomponent
	// Build and Run
	ModelComponent* model = new ModelComponent(L"Meshes/Chair.ovm");
	m_pChair->AddComponent(model);

	//ColorMaterial* colorMat = MaterialManager::Get()->CreateMaterial<ColorMaterial>();
	//.pMaterialManager->AddMaterial(new ColorMaterial(), 0);
	//model->SetMaterial(1);
	


	//4. Create a DiffuseMaterial (using PosNormTex3D.fx)
	//		Make sure you are able to set a texture (DiffuseMaterial::SetDiffuseTexture(const wstring& assetFile))
	//		Load the correct shadervariable and set it during the material variable update
	//5. Assign the material to the modelcomponent
	// Build and Run
	DiffuseMaterial* diffuse = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	diffuse->SetDiffuseMaterial(L"Textures/Chair_Dark.dds");
	model->SetMaterial(2);

	//6. Attach a rigidbody component (pure-dynamic)
	//7. Attach a collider component (Use a PxConvexMeshGeometry [chair.ovpc])
	// Build and Run

	RigidBodyComponent* pRigidBody{ new RigidBodyComponent{} };
	m_pChair->AddComponent(pRigidBody);
	
	m_pChair->GetTransform()->Translate(DirectX::XMVECTOR{ 0.f, 0.1f, 0.f });

	AddChild(m_pChair);
}

void ModelTestScene::Update()
{}

void ModelTestScene::Draw()
{}

void ModelTestScene::OnGUI()
{
}
