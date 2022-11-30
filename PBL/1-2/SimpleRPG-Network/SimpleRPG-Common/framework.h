#pragma once

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

#include <Windows.h>
#include <WinSock2.h>
#include <string>
#include <format>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <cstdint>
#include <climits>
#include <atomic>
#include <mutex>
#include <deque>
#include <uv.h>
#include <plog/Log.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Initializers/ConsoleInitializer.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Userenv.lib")
#pragma comment(lib, "Iphlpapi.lib")
#pragma comment(lib, "uv_a.lib")