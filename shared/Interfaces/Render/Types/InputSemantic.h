#pragma once

struct ZN_API InputSemantic
{
	InputSemantic()
		: Index(0)
		, GLType(0)
		, GLSize(0)
	{}

	InputSemantic(const std::string& name, uint32 index)
		: Name(name)
		, Index(index)
		, GLType(0)
		, GLSize(0)
	{}

	InputSemantic(const std::string& name, uint32 index, uint32 GLtype, uint32 GLsize)
		: Name(name)
		, Index(index)
		, GLType(GLtype)
		, GLSize(GLsize)
	{}

	inline bool operator==(const InputSemantic& rhs) const
	{
		return (Name == rhs.Name) && (Index == rhs.Index) && (GLType == rhs.GLType) && (GLSize == rhs.GLSize);
	}

	std::string Name;
	uint32 Index;
	uint32 GLType;
	uint32 GLSize;
};


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
}