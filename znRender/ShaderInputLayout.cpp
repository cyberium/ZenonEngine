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
bool IShaderInputLayout::HasSemantic(const BufferBinding& binding) const
{
    for (auto& it : m_InputSemantics)
    {
        // DIRECTX
        //if (it.first.Name == binding.Name /*&& it.first.Index == binding.Index*/)
        //    return true;

        // OPENGL
        if (it.first.Name == std::string(binding.Name + std::to_string(binding.Index)))
            return true;        
    }

    return false;
}

const InputSemantic& IShaderInputLayout::GetSemantic(const BufferBinding& binding) const
{
    for (auto& it : m_InputSemantics)
    {
        //if (it.first.Name == binding.Name /*&& it.first.Index == binding.Index*/)
        //    return it.first;

        // OPENGL
        if (it.first.Name == std::string(binding.Name + std::to_string(binding.Index)))
            return it.first;
    }

    _ASSERT(false);
    return gs_InvalidShaderSemantic;
}

UINT IShaderInputLayout::GetSemanticSlot(const BufferBinding& binding) const
{
    for (auto& it : m_InputSemantics)
    {
        //if (it.first.Name == binding.Name /*&& it.first.Index == binding.Index*/)
        //    return it.second;

        // OPENGL
        if (it.first.Name == std::string(binding.Name + std::to_string(binding.Index)))
            return it.second;
    }

    _ASSERT(false);
    return UINT_MAX;
}
