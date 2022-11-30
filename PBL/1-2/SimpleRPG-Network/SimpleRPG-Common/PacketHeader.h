#pragma once

#include <cstdint>

#pragma pack(push,2)
typedef struct
{
	uint16_t ContentLength;
	uint16_t MessageType;
} PacketHeader;
#pragma pack(pop)