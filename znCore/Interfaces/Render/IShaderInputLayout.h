#pragma once

ZN_INTERFACE OW_ENGINE_API IShaderInputLayout
{
	typedef std::unordered_map<InputSemantic, UINT> SemanticMap;

	virtual ~IShaderInputLayout() {}

	virtual bool                                    HasSemantic(const BufferBinding& binding) const = 0;
	virtual const InputSemantic&                    GetSemantic(const BufferBinding& binding) const = 0;
	virtual UINT                                    GetSemanticSlot(const BufferBinding& binding) const = 0;
	virtual const SemanticMap&                      GetSemantics() const = 0;
};