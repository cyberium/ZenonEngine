#pragma once

#include "common.h"

struct OW_ENGINE_API BufferBinding
{
    BufferBinding();
    BufferBinding(const std::string& name, uint32 index);
    virtual ~BufferBinding();

	// Provide the < operator for STL containers.
    bool operator<(const BufferBinding& rhs) const;

	std::string  Name;
	uint32       Index;
};
