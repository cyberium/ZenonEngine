#pragma once

// =================================================================================================
// USEFULL MACROS
// =================================================================================================

namespace std
{
	template<>
	struct hash<GUID>
	{
		inline size_t operator()(const GUID& guid) const noexcept
		{
			const std::uint64_t* p = reinterpret_cast<const std::uint64_t*>(&guid);
			std::hash<std::uint64_t> hash;
			return hash(p[0]) ^ hash(p[1]);
		}
	};
}
