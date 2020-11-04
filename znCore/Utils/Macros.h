#pragma once

// =================================================================================================
// USEFULL MACROS
// =================================================================================================

#define OW_UNUSED_VAR(a)     ((void)(a))
#define OW_ZERO_MEM(a)       (memset(a, 0, sizeof(a)));
#define OW_COUNT_ELEMENTS(a) ((sizeof(a)) / (sizeof(a[0])))

#define ERASE_VECTOR(_name)                                 \
for (auto it = _name.begin(); it != _name.end();)           \
{                                                           \
    auto obj = *it;                                         \
    it = _name.erase(it);                                   \
    delete obj;                                             \
}                                                           \
_name.clear()

#define ERASE_MAP(_name)                                    \
for (auto itr = _name.begin(); itr != _name.end(); itr++)   \
{                                                           \
	delete (itr->second);                                   \
}                                                           \
_name.clear();

// Helpers

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
