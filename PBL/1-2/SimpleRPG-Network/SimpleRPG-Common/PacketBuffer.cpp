#include "pch.h"
#include "PacketBuffer.h"

PacketBuffer::PacketBuffer()
	: Buffer()
	, Pos()
{
}

void PacketBuffer::Scan(size_t suggestedSize, uv_buf_t* buf)
{
	buf->base = &Buffer[Pos];
	buf->len = static_cast<ULONG>(Buffer.capacity() - Pos);
}

void PacketBuffer::Pull(size_t len)
{
	if (len > Pos)
	{
		throw std::out_of_range("buffer underflow");
	}
	Buffer.erase(Buffer.begin(), std::next(Buffer.begin(), len));
	Pos -= len;
}
