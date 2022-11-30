#pragma once

typedef struct PacketBuffer
{
	std::vector<char>	Buffer;
	size_t				Pos;

	PacketBuffer();

	size_t	Remaining() const { return Buffer.size() - Pos; };
	void	Scan(size_t suggestedSize, uv_buf_t* buf);
	void	Pull(size_t len);
} PacketBuffer;