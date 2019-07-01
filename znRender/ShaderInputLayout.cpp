#include "stdafx.h"

// General
#include "ShaderInputLayout.h"

static InputSemantic gs_InvalidShaderSemantic;

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
        if (it.first.Name == binding.Name && it.first.Index == binding.Index)
        {
            return true;
        }
    }

    return false;
}

const InputSemantic& IShaderInputLayout::GetSemantic(const BufferBinding& binding) const
{
    for (auto& it : m_InputSemantics)
    {
        if (it.first.Name == binding.Name && it.first.Index == binding.Index)
        {
            return it.first;
        }
    }

    _ASSERT(false);
    return gs_InvalidShaderSemantic;
}

UINT IShaderInputLayout::GetSemanticSlot(const BufferBinding& binding) const
{
    for (auto& it : m_InputSemantics)
    {
        if (it.first.Name == binding.Name && it.first.Index == binding.Index)
        {
            return it.second;
        }
    }

    _ASSERT(false);
    return UINT_MAX;
}
