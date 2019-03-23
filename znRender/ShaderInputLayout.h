#pragma once

#include "BufferBinding.h"
#include "InputSemantic.h"

class IShaderInputLayout : public Object
{
public:
	virtual ~IShaderInputLayout() {};


	// Check to see if this shader supports a given semantic.
	virtual bool                 HasSemantic(const BufferBinding& binding) const = 0;
	virtual const InputSemantic& GetSemantic(const BufferBinding& binding) const = 0;
	virtual UINT                 GetSemanticSlot(const BufferBinding& binding) const = 0;

protected:
	// A map to convert a vertex attribute semantic to a slot.
	typedef std::map<InputSemantic, UINT> SemanticMap;
	SemanticMap                  m_InputSemantics;
};