#include "stdafx.h"

// General
#include "ShaderInputLayoutBase.h"

static InputSemantic gs_InvalidShaderSemantic;

ShaderInputLayoutBase::ShaderInputLayoutBase()
{}

ShaderInputLayoutBase::~ShaderInputLayoutBase()
{}

// ShaderInputLayoutBase
const IShaderInputLayout::SemanticMap& ShaderInputLayoutBase::GetSemantics() const
{
    return m_InputSemantics;
}
