#pragma once

#include <AbstractSession.h>
#include <GameData.h>

class GameSession : public AbstractSession
{
protected:
	virtual void	Process(uint16_t msgType, InBuffer& inBuffer);

public:
	GameSession() : AbstractSession(), _saveName(), _savePassword() {};
	GameSession(uv_loop_t* loop) : AbstractSession(loop), _saveName(), _savePassword() {}
	virtual ~GameSession() {};
	GameSession(const GameSession&) = delete;
	GameSession(GameSession&&) = delete;
	GameSession& operator=(const GameSession&) = delete;
	GameSession& operator=(GameSession&&) = delete;

private:
	std::string	_saveName;
	std::string	_savePassword;

	int8_t	CheckAuth(const std::string& name, const std::string& password, std::ifstream& ifs) const;
	int8_t	LoadGame(GameData& data, const std::string& name, const std::string& password);
	int8_t	SaveGame(const GameData& data);
};
