#pragma once

struct OW_ENGINE_API InputSemantic
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

	// Provide the < operator for STL containers.
	bool operator<(const InputSemantic& rhs) const
	{
		if (Name < rhs.Name) return true;
		if (Name > rhs.Name) return false;
		// Names are equal...

		if (Index < rhs.Index) return true;
		if (Index > rhs.Index) return false;
		// Indexes are equal...

		return false;
	}

	std::string Name;
	uint32 Index;
	uint32 GLType;
	uint32 GLSize;
};
