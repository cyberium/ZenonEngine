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
    typedef std::map<InputSemantic, UINT> SemanticMap;
public:
                                                    IShaderInputLayout();
	virtual                                         ~IShaderInputLayout();

    // IShaderInputLayout
    virtual bool                                    HasSemantic(const BufferBinding& binding) const = 0;
    virtual const InputSemantic&                    GetSemantic(const BufferBinding& binding) const = 0;
    virtual UINT                                    GetSemanticSlot(const BufferBinding& binding) const = 0;
    virtual const SemanticMap&                      GetSemantics() const;

protected:
	SemanticMap                                     m_InputSemantics;
};