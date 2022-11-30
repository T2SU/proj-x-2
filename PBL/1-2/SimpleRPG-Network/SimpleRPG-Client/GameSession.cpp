#include "pch.h"
#include "GameSession.h"
#include <PacketHeader.h>
#include <SimpleRPG.h>
#include <InBuffer.h>

GameSession::~GameSession()
{
	PLOG_VERBOSE << "Closed connection";
	_connected = false;
	_saveAuthed = false;
	PLOG_ERROR << "서버와 연결이 끊어졌습니다.";
	exit(1);
}

void GameSession::Process(uint16_t msgType, InBuffer& inBuffer)
{
	switch (msgType)
	{
	case ServerToClient_LoadRes:
	{
		auto err = inBuffer.Read<int8_t>();
		switch (err)
		{
		case AuthError_Success:
			_gameData = inBuffer.Read<GameData>();
			_saveAuthed = true;
			std::cerr << "저장된 게임 진행을 서버에서 불러왔습니다." << std::endl;
			Set();
			break;
		case AuthError_NoAccount:
			std::cerr << "이름이 존재하지 않습니다." << std::endl;
			break;
		case AuthError_InvalidPassword:
			std::cerr << "패스워드가 틀렸습니다." << std::endl;
			break;
		case AuthError_InternalServerError:
			std::cerr << "서버 내부 오류가 발생했습니다." << std::endl;
			break;
		case AuthError_EmptyValue:
			std::cerr << "이름 또는 패스워드는 빈 문자열일 수 없습니다." << std::endl;
			break;
		}
		break;
	}
	case ServerToClient_SaveAuthRes:
	{
		auto err = inBuffer.Read<int8_t>();
		switch (err)
		{
		case AuthError_Success:
			_saveAuthed = true;
			RequestSave();
			return;
		case AuthError_NoAccount:
			std::cerr << "이름이 존재하지 않습니다." << std::endl;
			break;
		case AuthError_InvalidPassword:
			std::cerr << "패스워드가 틀렸습니다." << std::endl;
			break;
		case AuthError_InternalServerError:
			std::cerr << "서버 내부 오류가 발생했습니다." << std::endl;
			break;
		case AuthError_EmptyValue:
			std::cerr << "이름 또는 패스워드는 빈 문자열일 수 없습니다." << std::endl;
			break;
		}
		Set();
		break;
	}
	case ServerToClient_SaveRes:
	{
		auto err = inBuffer.Read<int8_t>();
		switch (err)
		{
		case AuthError_Success:
			std::cout << "저장이 완료되었습니다." << std::endl;
			break;
		default:
			std::cerr << "저장에 실패하였습니다." << std::endl;
			break;
		}
		Set();
		break;
	}
	}
}

void GameSession::OnConnect(uv_connect_t* con, int status)
{
	auto session = reinterpret_cast<GameSession*>(con->data);
	if (status >= 0)
	{
		std::cout << "서버에 연결되었습니다." << std::endl;
		session->_connected = true;
		uv_read_start(reinterpret_cast<uv_stream_t*>(&session->_client), AbstractSession::AllocBuffer, AbstractSession::OnRead);
	}
	else
	{
		session->_connected = false;
		std::cerr << "Failed to connect: " << uv_strerror(status) << std::endl;
	}
	delete con;
	session->Set();
}

void GameSession::Connect(uv_loop_t* loop, const char* host, uint16_t port)
{
	_client.data = this;
	uv_tcp_init(loop, &_client);
	uv_tcp_keepalive(&_client, 1, 60);
	uv_ip4_addr(host, port, &_target);

	auto con = new uv_connect_t;
	con->data = this;
	uv_tcp_connect(con, reinterpret_cast<uv_tcp_t*>(&_client), reinterpret_cast<const sockaddr*>(&_target), OnConnect);
}

void GameSession::Wait()
{
	std::unique_lock<std::mutex> lock(_mutex);
	while (!_signaled)
	{
		_conval.wait(lock);
	}
}

void GameSession::Set()
{
	{
		std::unique_lock<std::mutex> lock(_mutex);
		_signaled = true;
	}
	_conval.notify_all();
}

void GameSession::Unset()
{
	std::unique_lock<std::mutex> lock(_mutex);
	_signaled = false;
}

void GameSession::RequestSave()
{
	OutBuffer outBuffer;
	outBuffer.Write(_gameData);
	Send(ClientToServer_SaveReq, outBuffer);
}