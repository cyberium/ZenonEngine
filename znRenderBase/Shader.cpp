#include "stdafx.h"

// General
#include "Shader.h"

Shader::Shader() :
	m_ShaderType(ShaderType::UnknownShaderType)
{}

Shader::~Shader()
{}

Shader::ShaderType Shader::GetType() const
{
	return m_ShaderType;
}
