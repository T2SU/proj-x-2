#include "pch.h"
#include "InBuffer.h"

InBuffer::InBuffer(const char* buf, size_t len)
	: _stream(std::string(buf, len))
	, _serializer(_stream)
{
}
