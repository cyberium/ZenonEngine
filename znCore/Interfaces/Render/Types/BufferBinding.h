#pragma once

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

	inline bool operator==(const BufferBinding& rhs) const
	{
		return (Name == rhs.Name) && (Index == rhs.Index);
	}

	std::string  Name;
	uint32       Index;
};

namespace std
{
	template<>
	struct hash<BufferBinding>
	{
		size_t operator()(const BufferBinding& buffer) const noexcept
		{
			std::hash<std::string> hash;
			return hash(buffer.Name + std::to_string(buffer.Index));
		}
	};
}