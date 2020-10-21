#pragma once

struct ZN_API InputSemantic
{
	InputSemantic()
		: Index()
	{}

	InputSemantic(const std::string& Name, uint32 Index)
		: Name(Name)
		, Index(Index)
	{}

	inline bool operator==(const InputSemantic& rhs) const
	{
		return (Name == rhs.Name) && (Index == rhs.Index);
	}

	std::string Name;
	uint32 Index;
};


struct InputSemanticOpenGL
	: InputSemantic
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

/*
namespace std
{
	template<>
	struct hash<InputSemantic>
	{
		size_t operator()(const InputSemantic& buffer) const noexcept
		{
			std::hash<std::string> hash;
			return hash(buffer.Name + std::to_string(buffer.Index));
		}
	};
}*/