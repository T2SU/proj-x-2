#include "pch.h"
#include "OutBuffer.h"

OutBuffer::OutBuffer()
	: _stream()
	, _serializer(_stream)
	, _cacheBuffer()
	, _dirtyCache()
{
}

void OutBuffer::UpdateBuffer() const
{
	if (!_dirtyCache) return;
	_cacheBuffer = _stream.str();
	_dirtyCache = false;
}