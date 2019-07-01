#include "stdafx.h"

// General
#include "BufferBinding.h"

BufferBinding::BufferBinding()
    : Index(0)
{}

BufferBinding::BufferBinding(const std::string& name, uint32 index)
    : Name(name)
    , Index(index)
{}

BufferBinding::~BufferBinding()
{

}

// Provide the < operator for STL containers.
bool BufferBinding::operator<(const BufferBinding& rhs) const
{
    if (Name < rhs.Name) return true;
    if (Name > rhs.Name) return false;

    if (Index < rhs.Index) return true;
    if (Index > rhs.Index) return false;

    return false;
}