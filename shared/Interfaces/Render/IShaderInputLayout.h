#pragma once

ZN_INTERFACE ZN_API IShaderInputLayout
{
	virtual ~IShaderInputLayout() {}

	virtual bool                                    HasSemantic(const BufferBinding& binding) const = 0;
	virtual const InputSemantic&                    GetSemantic(const BufferBinding& binding) const = 0;
	virtual UINT                                    GetSemanticSlot(const BufferBinding& binding) const = 0;

	virtual size_t                                  GetSemanticsCount() const = 0;
	virtual const InputSemantic&                    GetSemantic(size_t Index) const = 0;
	virtual UINT                                    GetSemanticSlot(size_t Index) const = 0;
};