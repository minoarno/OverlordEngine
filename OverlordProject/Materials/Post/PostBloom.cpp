#include "stdafx.h"
#include "PostBloom.h"

PostBloom::PostBloom() :
	PostProcessingMaterial(L"Effects/Post/Bloom.fx")
{
}

void PostBloom::SetThreshold(float value)
{
	m_pBaseEffect->GetVariableByName("gThreshold")->AsScalar()->SetFloat(value);
}

void PostBloom::SetStrength(float value)
{
	m_pBaseEffect->GetVariableByName("gStrength")->AsScalar()->SetFloat(value);
}
