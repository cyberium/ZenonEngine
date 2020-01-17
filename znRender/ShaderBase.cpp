#include "stdafx.h"

// General
#include "ShaderBase.h"

// Additional
#include "ShaderParameterBase.h"

static std::shared_ptr<IShaderParameter> gs_InvalidShaderParameter;

ShaderBase::ShaderBase() :
	m_ShaderType(EShaderType::UnknownShaderType)
{
	gs_InvalidShaderParameter = std::make_shared<ShaderParameterBase>();
}

ShaderBase::~ShaderBase()
{}

EShaderType ShaderBase::GetType() const
{
	return m_ShaderType;
}

std::shared_ptr<IShaderInputLayout> ShaderBase::GetInputLayout() const
{
    return m_InputLayout;
}

const std::shared_ptr<IShaderParameter>& ShaderBase::GetShaderParameterByName(const std::string& name) const
{
	const auto& iter = m_ShaderParameters.find(name);
    if (iter != m_ShaderParameters.end())
        return iter->second;

    return gs_InvalidShaderParameter;
}