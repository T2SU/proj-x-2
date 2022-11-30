#pragma once

#include "PacketBuffer.h"
#include "OutBuffer.h"

class InBuffer;
class OutBuffer;

typedef struct WriteReq
{
	// uv_write_t: 전송 핸들
	// uv_buf_t: 보낼 데이터
	uv_write_t	Req;
	uv_buf_t	Buf;
	char* Data;
} WriteReq;

class AbstractSession
{
protected:
	uv_tcp_t		_client;
	PacketBuffer	_buffer;

private:
	uint32_t		_socketId;

	static void		OnSend(uv_write_t* req, int status);

protected:
	virtual void	Process(uint16_t msgType, InBuffer& inBuffer) = 0;
	AbstractSession();

	static std::string Dump(const char* buf, size_t len);

public:
	AbstractSession(uv_loop_t* loop);
	virtual ~AbstractSession();
	AbstractSession(const AbstractSession&) = delete;
	AbstractSession(AbstractSession&&) = delete;
	AbstractSession& operator=(const AbstractSession&) = delete;
	AbstractSession& operator=(AbstractSession&&) = delete;

	void		Recv(const char* buf, size_t len);
	void		Send(uint16_t msgType, OutBuffer& outBuffer);

	uv_tcp_t*	GetHandle() { return &_client; }
	uint32_t	GetSocketId() const { return _socketId; }
	void		Close();



	static void AllocBuffer(uv_handle_t* handle, size_t suggestedSize, uv_buf_t* buf);
	static void OnRead(uv_stream_t* client, ssize_t read, const uv_buf_t* buf);
	static void OnClose(uv_handle_t* handle);
};