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
	PLOG_ERROR << "������ ������ ���������ϴ�.";
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
			std::cerr << "����� ���� ������ �������� �ҷ��Խ��ϴ�." << std::endl;
			Set();
			break;
		case AuthError_NoAccount:
			std::cerr << "�̸��� �������� �ʽ��ϴ�." << std::endl;
			break;
		case AuthError_InvalidPassword:
			std::cerr << "�н����尡 Ʋ�Ƚ��ϴ�." << std::endl;
			break;
		case AuthError_InternalServerError:
			std::cerr << "���� ���� ������ �߻��߽��ϴ�." << std::endl;
			break;
		case AuthError_EmptyValue:
			std::cerr << "�̸� �Ǵ� �н������ �� ���ڿ��� �� �����ϴ�." << std::endl;
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
			std::cerr << "�̸��� �������� �ʽ��ϴ�." << std::endl;
			break;
		case AuthError_InvalidPassword:
			std::cerr << "�н����尡 Ʋ�Ƚ��ϴ�." << std::endl;
			break;
		case AuthError_InternalServerError:
			std::cerr << "���� ���� ������ �߻��߽��ϴ�." << std::endl;
			break;
		case AuthError_EmptyValue:
			std::cerr << "�̸� �Ǵ� �н������ �� ���ڿ��� �� �����ϴ�." << std::endl;
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
			std::cout << "������ �Ϸ�Ǿ����ϴ�." << std::endl;
			break;
		default:
			std::cerr << "���忡 �����Ͽ����ϴ�." << std::endl;
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
		std::cout << "������ ����Ǿ����ϴ�." << std::endl;
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