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

