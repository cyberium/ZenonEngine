#include "stdafx.h"

// Include 
#include "RenderDevice.h"

// General
#include "ShaderInputLayout.h"

static InputSemantic gs_InvalidShaderSemantic;

IShaderInputLayout::IShaderInputLayout()
{
}

IShaderInputLayout::~IShaderInputLayout()
{
}



//
// IShaderInputLayout
//
const IShaderInputLayout::SemanticMap& IShaderInputLayout::GetSemantics() const
{
    return m_InputSemantics;
}
