#include "stdafx.h"

// General
#include "Shader.h"

static ShaderParameter gs_InvalidShaderParameter;

Shader::Shader() :
	m_ShaderType(ShaderType::UnknownShaderType)
{}

Shader::~Shader()
{}

Shader::ShaderType Shader::GetType() const
{
	return m_ShaderType;
}

std::shared_ptr<IShaderInputLayout> Shader::GetInputLayout() const
{
    return m_InputLayout;
}

ShaderParameter& Shader::GetShaderParameterByName(const std::string& name) const
{
    ParameterMap::const_iterator iter = m_ShaderParameters.find(name);
    if (iter != m_ShaderParameters.end())
    {
        return *(iter->second);
    }

    _ASSERT(false);
    return gs_InvalidShaderParameter;
}