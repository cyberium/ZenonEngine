#pragma once

#include "common.h"

#include "BufferBinding.h"
#include "InputSemantic.h"

// FORWARD BEGIN
class IRenderDevice;
// FORWARD END

class OW_ENGINE_API IShaderInputLayout : public Object
{
public:
    IShaderInputLayout();
	virtual ~IShaderInputLayout();

    // IShaderInputLayout
    bool                                            HasSemantic(const BufferBinding& binding) const;
    const InputSemantic&                            GetSemantic(const BufferBinding& binding) const;
    UINT                                            GetSemanticSlot(const BufferBinding& binding) const;

protected:
	// A map to convert a vertex attribute semantic to a slot.
	typedef std::map<InputSemantic, UINT> SemanticMap;
	SemanticMap                                     m_InputSemantics;
};