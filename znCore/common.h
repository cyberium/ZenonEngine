#pragma once

#include <algorithm>
#include <codecvt>
#include <functional>
#include <fstream>
#include <cmath>
#include <memory>

#include <mutex>
#include <atomic>
#include <future>
#include <chrono>

// Macros
#include "Debug.h"
#include "Macros.h"

// Primitives
#include "Math.h"
#include "BoundingBox.h"
#include "BoundingRect.h"
#include "BoundingSphere.h"
#include "Frustrum.h"
#include "Plane.h"
#include "Rect.h"

// Additional types
#include "Color.h"
#include "Timer.h"
#include "HighResolutionTimer.h"
#include "Resource.h"

// Utils
#include "Random.h"
#include "MapAssync.h"
#include "LockedQueue.h"
#include "Utils.h"
#include "Log.h"