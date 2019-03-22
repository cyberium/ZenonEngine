#pragma once

#include "MathCommon.h"

static inline quat Fix_XZmYW(const quat& _quat)
{
	return quat(_quat.w, _quat.x, _quat.z, -_quat.y);
}