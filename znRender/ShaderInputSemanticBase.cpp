#include "stdafx.h"

// General
#include "ShaderInputSemanticBase.h"

ShaderInputSemanticBase::ShaderInputSemanticBase(const std::string & Name, uint32 Index)
	: m_Name(Name)
	, m_Index(Index)
{
}

ShaderInputSemanticBase::~ShaderInputSemanticBase()
{
}



//
// IShaderInputSemantic
//
const std::string & ShaderInputSemanticBase::GetName() const
{
	return m_Name;
}

uint32 ShaderInputSemanticBase::GetIndex() const
{
	return m_Index;
}

bool ShaderInputSemanticBase::IsEqual(const IShaderInputSemantic * Other) const
{
	return GetName() == Other->GetName() && GetIndex() == Other->GetIndex();
}
