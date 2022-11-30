#include "pch.h"
#include "AbstractSession.h"
#include "InBuffer.h"
#include "PacketHeader.h"
#include "SimpleRPG.h"

std::atomic_uint32_t a(1);

AbstractSession::AbstractSession()
	: _client()
	, _socketId()
	, _buffer()
{
}

AbstractSession::AbstractSession(uv_loop_t* loop)
	: _client()
	, _socketId(a.fetch_add(1, std::memory_order_relaxed))
	, _buffer()
{
	_client.data = this;
	uv_tcp_init(loop, reinterpret_cast<uv_tcp_t*>(&_client));
}

AbstractSession::~AbstractSession()
{
	_socketId = 0;
}

std::string AbstractSession::Dump(const char* buf, size_t len)
{
	bool first = true;
	std::ostringstream result;
	result << "Data=[";
	for (size_t i = 0; i < len; i++)
	{
		const auto x = buf[i];
		if (!first)
		{
			result << " ";
		}
		result << std::format("{:02X}", static_cast<unsigned long>(x));
		first = false;
	}
	result << "]";
	return result.str();
}

void AbstractSession::Recv(const char* buf, size_t len)
{
	_buffer.Buffer.insert(_buffer.Buffer.end(), &buf[0], &buf[len]);
	_buffer.Pos += len;

	while (true)
	{
		const auto header = reinterpret_cast<PacketHeader*>(_buffer.Buffer.data());

		// Check Size of Packet Buffer
		if (_buffer.Buffer.size() < sizeof(PacketHeader)) return;
		if (_buffer.Buffer.size() < sizeof(PacketHeader) + header->ContentLength) return;

		// Decrypt Packet (Placeholder)
		for (size_t i = 0; i < header->ContentLength; i++)
		{
			_buffer.Buffer[sizeof(PacketHeader) + i] ^= 0xAA;
		}

		// Process Message
		InBuffer inBuffer(
			header->ContentLength ? &_buffer.Buffer[sizeof(PacketHeader)] : nullptr, 
			header->ContentLength);
		try
		{
			Process(header->MessageType, inBuffer);
		}
		catch (...)
		{
			PLOG_ERROR << "Packet Process Error: " << Dump(&_buffer.Buffer[sizeof(PacketHeader)], header->ContentLength);
		}

		_buffer.Pull(sizeof(PacketHeader) + header->ContentLength);
	}
}

void AbstractSession::OnSend(uv_write_t* req, int status)
{
	if (status)
	{
		PLOG_ERROR << "Send packet error: " << uv_strerror(status);
	}
	auto wq = reinterpret_cast<WriteReq*>(req->data);
	delete[] wq->Data;
	delete wq;
}

void AbstractSession::Send(uint16_t msgType, OutBuffer& outBuffer)
{
	auto req = new WriteReq;
	req->Req.data = req;
	req->Data = new char[sizeof(PacketHeader) + outBuffer.GetSize()];
	PacketHeader* header = reinterpret_cast<PacketHeader*>(req->Data);

	// Set Packet Header
	header->MessageType = msgType;
	header->ContentLength = static_cast<uint16_t>(outBuffer.GetSize());

	// Set Packet Content
	std::memcpy(&req->Data[sizeof(PacketHeader)], outBuffer.GetBytes(), outBuffer.GetSize());

	// Decrypt Packet (Placeholder)
#pragma warning(push)
#pragma warning(disable: 6385) // False Positive
	for (auto i = 0; i < outBuffer.GetSize(); i++)
	{
		req->Data[sizeof(PacketHeader) + i] ^= 0xAA;
	}
#pragma warning(pop)

	// Write
	const auto len = static_cast<unsigned int>(sizeof(PacketHeader) + outBuffer.GetSize());
	req->Buf = uv_buf_init(req->Data, len);
	uv_write(
		reinterpret_cast<uv_write_t*>(&req->Req),
		reinterpret_cast<uv_stream_t*>(&_client),
		&req->Buf, 1, OnSend);
}

void AbstractSession::Close()
{
	uv_close(reinterpret_cast<uv_handle_t*>(&_client), nullptr);
	this->~AbstractSession();
}



void AbstractSession::AllocBuffer(uv_handle_t* handle, size_t suggestedSize, uv_buf_t* buf)
{
	buf->base = new char[suggestedSize];
	buf->len = static_cast<ULONG>(suggestedSize);
}

void AbstractSession::OnRead(uv_stream_t* client, ssize_t read, const uv_buf_t* buf)
{
	if (read > 0)
	{
		reinterpret_cast<AbstractSession*>(client->data)->Recv(buf->base, read);
	}
	else if (read < 0)
	{
		if (read != UV_EOF)
		{
			PLOG_ERROR << "Read error: " << uv_err_name(static_cast<int>(read));
		}
		uv_close(reinterpret_cast<uv_handle_t*>(client), OnClose);
	}
	delete[] buf->base;
}

void AbstractSession::OnClose(uv_handle_t* handle)
{
	auto session = reinterpret_cast<AbstractSession*>(handle->data);
	PLOG_INFO << "Closed connection [" << session->GetSocketId() << "]";
	session->~AbstractSession();
}