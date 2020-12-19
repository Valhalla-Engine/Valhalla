#pragma once

#include <iostream>
#pragma once

#ifdef VH_PLATFORM_WINDOWS
#include <Windows.h>
#endif

#include <memory>
#include <vector>
#include <string>
#include <array>
#include <unordered_map>
#include <functional>
#include <algorithm>

#include <fstream>


#include "Valhalla/Core/Base.h"
#include "Valhalla/Core/Log.h"
#include "Valhalla/Core/Events/Event.h"

#ifdef VH_PLATFORM_WINDOWS
	#include <Windows.h>
#endif