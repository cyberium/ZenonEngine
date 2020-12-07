#pragma once

ZN_INTERFACE ZN_API IShaderInputSemantic
{
	virtual ~IShaderInputSemantic() {}

	virtual const std::string& GetName() const = 0;
	virtual uint32 GetIndex() const = 0;

	virtual bool IsEqual(const IShaderInputSemantic* Other) const = 0;

	inline bool operator==(const IShaderInputSemantic& rhs) const
	{
		return IsEqual(&rhs);
	}
	inline bool operator==(const IShaderInputSemantic* rhs) const
	{
		return IsEqual(rhs);
	}
};


/*
struct InputSemanticOpenGL
	: public InputSemantic
{
	InputSemanticOpenGL(const std::string& Name, uint32 Index, uint32 GLtype, uint32 GLsize)
		: InputSemantic(Name, Index)
		, GLType(GLtype)
		, GLSize(GLsize)
	{}

	inline bool operator==(const InputSemanticOpenGL& rhs) const
	{
		return __super::operator==(rhs) && (GLType == rhs.GLType) && (GLSize == rhs.GLSize);
	}

	uint32 GLType;
	uint32 GLSize;
};
*/

ZN_INTERFACE ZN_API IShaderInputLayout
{
	virtual ~IShaderInputLayout() {}

	virtual bool                                    HasSemantic(const BufferBinding& binding) const = 0;
	virtual const IShaderInputSemantic*             GetSemantic(const BufferBinding& binding) const = 0;
	virtual uint32                                  GetSemanticSlot(const BufferBinding& binding) const = 0;

	virtual size_t                                  GetSemanticsCount() const = 0;
	virtual const IShaderInputSemantic*             GetSemantic(size_t Index) const = 0;
	virtual uint32                                  GetSemanticSlot(size_t Index) const = 0;
};