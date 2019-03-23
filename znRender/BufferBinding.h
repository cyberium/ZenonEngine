#pragma once

struct BufferBinding
{
	BufferBinding()
		: Index(0)
	{}

	BufferBinding(cstring name, uint32 index)
		: Name(name)
		, Index(index)
	{}

	// Provide the < operator for STL containers.
	bool operator<(const BufferBinding& rhs) const
	{
		if (Name < rhs.Name) return true;
		if (Name > rhs.Name) return false;

		if (Index < rhs.Index) return true;
		if (Index > rhs.Index) return false;

		return false;
	}

	std::string  Name;
	uint32       Index;
};
