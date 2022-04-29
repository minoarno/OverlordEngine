#include "stdafx.h"
#include "ModelAnimator.h"

ModelAnimator::ModelAnimator(MeshFilter* pMeshFilter):
	m_pMeshFilter{pMeshFilter}
{
	SetAnimation(0);
}

void ModelAnimator::Update(const SceneContext& sceneContext)
{
	//TODO_W7_();

	//We only update the transforms if the animation is running and the clip is set
	if (m_IsPlaying && m_ClipSet)
	{
		//1. 
		//Calculate the passedTicks (see the lab document)
		//auto passedTicks = ...
		//Make sure that the passedTicks stay between the m_CurrentClip.Duration bounds (fmod)
		auto passedTicks = sceneContext.pGameTime->GetElapsed() * m_CurrentClip.ticksPerSecond * m_AnimationSpeed;
		if(passedTicks > m_CurrentClip.duration)
		{
			passedTicks = m_CurrentClip.duration;
		}

		//2. 
		//IF m_Reversed is true
		//	Subtract passedTicks from m_TickCount
		//	If m_TickCount is smaller than zero, add m_CurrentClip.Duration to m_TickCount
		//ELSE
		//	Add passedTicks to m_TickCount
		//	if m_TickCount is bigger than the clip duration, subtract the duration from m_TickCount
		if (m_Reversed)
		{
			m_TickCount -= passedTicks;
			if (m_TickCount < 0.f)
			{
				m_TickCount += m_CurrentClip.duration;
			}
		}
		else
		{
			m_TickCount += passedTicks;
			if (m_TickCount > m_CurrentClip.duration)
			{
				m_TickCount -= m_CurrentClip.duration;
			}
		}

		//3.
		//Find the enclosing keys
		AnimationKey keyA{ m_CurrentClip.keys.front() };
		AnimationKey keyB{ m_CurrentClip.keys.front() };
		//Iterate all the keys of the clip and find the following keys:
		//keyA > Closest Key with Tick before/smaller than m_TickCount
		//keyB > Closest Key with Tick after/bigger than m_TickCount
		for (auto& key : m_CurrentClip.keys)
		{
			if (key.tick > m_TickCount)
			{
				keyB = key;
				break;
			}
			keyA = key;
		}

		//4.
		//Interpolate between keys
		//Figure out the BlendFactor (See lab document)
		auto blendB{ (m_TickCount - keyA.tick) / (keyB.tick - keyA.tick) };

		//Clear the m_Transforms vector
		m_Transforms.clear();

		//FOR every boneTransform in a key (So for every bone)
		//	Retrieve the transform from keyA (transformA)
		//	auto transformA = ...
		// 	Retrieve the transform from keyB (transformB)
		//	auto transformB = ...
		//	Decompose both transforms
		//	Lerp between all the transformations (Position, Scale, Rotation)
		//	Compose a transformation matrix with the lerp-results
		//	Add the resulting matrix to the m_Transforms vector
		auto boneCount{ int(keyA.boneTransforms.size()) };
		for (int bone{ 0 }; bone < boneCount; ++bone)
		{
			//	Retrieve the transform from keyA (transformA)
			auto trsA{ keyA.boneTransforms[bone] };
			// 	Retrieve the transform from keyB (transformB)
			auto trsB{ keyB.boneTransforms[bone] };
			//	Decompose both transforms
			auto matrixA{ DirectX::XMLoadFloat4x4(&trsA) };
			DirectX::XMVECTOR vecScaleA, vecRotA, vecTransA;
			DirectX::XMMatrixDecompose(&vecScaleA, &vecRotA, &vecTransA, matrixA);
			auto matrixB{ DirectX::XMLoadFloat4x4(&trsB) };
			DirectX::XMVECTOR vecScaleB, vecRotB, vecTransB;
			DirectX::XMMatrixDecompose(&vecScaleB, &vecRotB, &vecTransB, matrixB);
			//	Lerp between all the transformations (Position, Scale, Rotation)
			auto vecScaleAB{ DirectX::XMVectorLerp(vecScaleA, vecScaleB, blendB) };
			auto vecTransAB{ DirectX::XMVectorLerp(vecTransA, vecTransB, blendB) };
			auto vecRotAB{ DirectX::XMQuaternionSlerp(vecRotA, vecRotB, blendB) };
			//	Compose a transformation matrix with the lerp-results
			auto resultMatrix{ DirectX::XMMatrixTransformation(
				DirectX::FXMVECTOR{},
				DirectX::FXMVECTOR{},
				vecScaleAB,
				DirectX::FXMVECTOR{},
				vecRotAB,
				vecTransAB
			) };
			//	Add the resulting matrix to the m_Transforms vector
			DirectX::XMFLOAT4X4 resultFloat4x4{};
			DirectX::XMStoreFloat4x4(&resultFloat4x4, resultMatrix);
			m_Transforms.push_back(resultFloat4x4);
		}
	}
}

void ModelAnimator::SetAnimation(const std::wstring& clipName)
{
	//TODO_W7_()
	//Set m_ClipSet to false
	//Iterate the m_AnimationClips vector and search for an AnimationClip with the given name (clipName)
	//If found,
	//	Call SetAnimation(Animation Clip) with the found clip
	//Else
	//	Call Reset
	//	Log a warning with an appropriate message
	m_ClipSet = false;
	for (auto& clip : m_pMeshFilter->m_AnimationClips)
	{
		if (clip.name.compare(clipName) == 0)
		{
			SetAnimation(clip);
			return;
		}
	}
	Reset();
	Logger::LogWarning(L"Clipname {} not found", clipName);
}

void ModelAnimator::SetAnimation(UINT clipNumber)
{
	//TODO_W7_()
	//Set m_ClipSet to false
	m_ClipSet = false;
	
	//Check if clipNumber is smaller than the actual m_AnimationClips vector size
	//If not,
		//	Call Reset
		//	Log a warning with an appropriate message
		//	return
	//else
		//	Retrieve the AnimationClip from the m_AnimationClips vector based on the given clipNumber
		//	Call SetAnimation(AnimationClip clip)

	if (clipNumber < m_pMeshFilter->m_AnimationClips.size())
	{
		SetAnimation(m_pMeshFilter->m_AnimationClips[clipNumber]);
		return;
	}
	Reset();
	Logger::LogWarning(L"Animation ID is out of bounds");
}

void ModelAnimator::SetAnimation(const AnimationClip& clip)
{
	//TODO_W7_()
	//Set m_ClipSet to true
	//Set m_CurrentClip
	m_ClipSet = true;
	m_CurrentClip = clip;

	//Call Reset(false)
	Reset(false);
}

void ModelAnimator::Reset(bool pause)
{
	//TODO_W7_()
	//If pause is true, set m_IsPlaying to false
	if (pause)	m_IsPlaying = false;

	//Set m_TickCount to zero
	//Set m_AnimationSpeed to 1.0f
	m_TickCount = 0.f;
	m_AnimationSpeed = 1.f;

	//If m_ClipSet is true
	//	Retrieve the BoneTransform from the first Key from the current clip (m_CurrentClip)
	//	Refill the m_Transforms vector with the new BoneTransforms (have a look at vector::assign)
	//Else
	//	Create an IdentityMatrix 
	//	Refill the m_Transforms vector with this IdenityMatrix (Amount = BoneCount) (have a look at vector::assign)
	if (m_ClipSet)
	{
		m_Transforms.assign(m_CurrentClip.keys[0].boneTransforms.begin(), m_CurrentClip.keys[0].boneTransforms.end());
	}
	else
	{
		DirectX::XMFLOAT4X4 identity{ 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f };
		m_Transforms.assign(m_pMeshFilter->m_BoneCount, identity);
	}
}
