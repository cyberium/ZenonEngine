#pragma once

#include "common.h"

struct OW_ENGINE_API BufferBinding
{
    BufferBinding()
		: Index(0)
	{}
    BufferBinding(const std::string& name, uint32 index)
		: Name(name)
		, Index(index)
	{}
    virtual ~BufferBinding()
	{}

	inline bool operator<(const BufferBinding& rhs) const
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
