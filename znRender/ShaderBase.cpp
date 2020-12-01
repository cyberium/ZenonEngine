#include "stdafx.h"

// General
#include "ShaderBase.h"

// Additional
#include "ShaderParameterBase.h"

ShaderBase::ShaderBase() 
	: m_Type(EShaderType::UnknownShaderType)
{
}

ShaderBase::~ShaderBase()
{}

EShaderType ShaderBase::GetShaderType() const
{
	return m_Type;
}

IShaderInputLayout& ShaderBase::GetInputLayout() const
{
    return *m_InputLayout;
}

IShaderParameter* ShaderBase::GetShaderParameterByName(const std::string& name) const
{
	const auto& iter = m_ShaderParameters.find(name);
    if (iter == m_ShaderParameters.end())
		return nullptr;

    return iter->second.get();    
}