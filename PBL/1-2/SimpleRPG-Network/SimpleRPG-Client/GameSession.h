#pragma once

#include <AbstractSession.h>
#include <GameData.h>

class GameSession;

typedef struct ConnectReq
{
	uv_connect_t con;
	GameSession* session;
} ConnectReq;

class GameSession : public AbstractSession
{
private:
	std::mutex				_mutex;
	std::condition_variable	_conval;
	bool					_signaled;

	bool					_connected;
	bool					_saveAuthed;
	GameData				_gameData;

	static void		OnConnect(uv_connect_t* con, int status);

protected:
	sockaddr_in		_target;

	virtual void	Process(uint16_t msgType, InBuffer& inBuffer);

public:
	GameSession()
		: AbstractSession()
		, _mutex()
		, _conval()
		, _signaled()
		, _connected()
		, _saveAuthed()
		, _gameData()
		, _target() {}

	virtual ~GameSession();
	GameSession(const GameSession&) = delete;
	GameSession(GameSession&&) = delete;
	GameSession& operator=(const GameSession&) = delete;
	GameSession& operator=(GameSession&&) = delete;

	void	Connect(uv_loop_t* loop, const char* host, uint16_t port);
	void	Wait();
	void	Set();
	void	Unset();
	void	RequestSave();

	bool		IsConnected() const { return _connected; }
	bool		IsSaveAuthed() const { return _saveAuthed; }
	GameData&	GetGameData() { return _gameData; }
};
