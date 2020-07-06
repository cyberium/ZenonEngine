#include "stdafx.h"

// General
#include "ShaderBase.h"

// Additional
#include "ShaderParameterBase.h"

static ShaderParameterBase gs_InvalidShaderParameter;

ShaderBase::ShaderBase() :
	m_ShaderType(EShaderType::UnknownShaderType)
{
}

ShaderBase::~ShaderBase()
{}

EShaderType ShaderBase::GetShaderType() const
{
	return m_ShaderType;
}

IShaderInputLayout& ShaderBase::GetInputLayout() const
{
    return *m_InputLayout;
}

IShaderParameter& ShaderBase::GetShaderParameterByName(const std::string& name) const
{
	const auto& iter = m_ShaderParameters.find(name);
    if (iter == m_ShaderParameters.end())
		return gs_InvalidShaderParameter;

    return *iter->second;    
}