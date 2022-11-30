#pragma once

typedef struct WriteReq
{
	uv_write_t	Req;
	void*		Data;
	uv_buf_t	Buf;
} WriteReq;