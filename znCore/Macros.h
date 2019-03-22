#pragma once

// =================================================================================================
// Build settings
// =================================================================================================

#define MDX_PARTICLES_ENABLE

#define CLASSIC    0
#define BC         1
#define WOTLK      2
#define CATACLYSM  3

#define CURRENT    CLASSIC

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

