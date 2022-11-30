#include "pch.h"
#include "GameSession.h"
#include <PacketHeader.h>
#include <SimpleRPG.h>
#include <InBuffer.h>
#include <TinySHA1.hpp>

void GameSession::Process(uint16_t msgType, InBuffer& inBuffer)
{
	switch (msgType)
	{
	case ClientToServer_LoadReq:
	{
		OutBuffer outBuffer;
		GameData data;

		const auto name = inBuffer.ReadString();
		const auto password = inBuffer.ReadString();
		const auto result = LoadGame(data, name, password);

		outBuffer.Write<int8_t>(result);
		if (result == AuthError_Success)
		{
			_saveName = name;
			_savePassword = password;
			outBuffer.Write(data);
		}
		Send(ServerToClient_LoadRes, outBuffer);
		break;
	}
	case ClientToServer_SaveAuthReq:
	{
		OutBuffer outBuffer;
		std::ifstream ifs;

		const auto name = inBuffer.ReadString();
		const auto password = inBuffer.ReadString();
		const auto result = CheckAuth(name, password, ifs);
		if (result == AuthError_NoAccount || result == AuthError_Success)
		{
			outBuffer.Write<int8_t>(AuthError_Success);
			_saveName = name;
			_savePassword = password;
		}
		else
		{
			outBuffer.Write<int8_t>(result);
		}
		Send(ServerToClient_SaveAuthRes, outBuffer);
		break;
	}
	case ClientToServer_SaveReq:
	{
		OutBuffer outBuffer;
		if (_saveName.empty() || _savePassword.empty())
		{
			outBuffer.Write<int8_t>(AuthError_InternalServerError);
		}
		else
		{
			SaveGame(inBuffer.Read<GameData>());
			outBuffer.Write<int8_t>(AuthError_Success);
		}
		Send(ServerToClient_SaveRes, outBuffer);
		break;
	}

	}
}

static std::string Hash(const std::string& val)
{
	sha1::SHA1 s;
	s.processBytes(val.c_str(), val.size());
	uint32_t digest[5];
	s.getDigest(digest);
	char tmp[41];
	snprintf(tmp, 40, "%08x%08x%08x%08x%08x", digest[0], digest[1], digest[2], digest[3], digest[4]);
	return std::string(tmp, 40);
}

int8_t GameSession::CheckAuth(const std::string& name, const std::string& password, std::ifstream& ifs) const
{
	if (name.empty() || password.empty()) return AuthError_EmptyValue;

	ifs = std::ifstream(Hash(name), std::ios::binary);
	if (!ifs) return AuthError_NoAccount;
	ifs.exceptions(ifs.exceptions() | std::ios::failbit);

	uint16_t length;
	ifs.read(reinterpret_cast<char*>(&length), 2);

	std::string buffer(length, 0);
	ifs.read(buffer.data(), length);

	if (password != buffer) return AuthError_InvalidPassword;

	return AuthError_Success;
}

int8_t GameSession::LoadGame(GameData& data, const std::string& name, const std::string& password)
{
	std::ifstream ifs;
	auto result = CheckAuth(name, password, ifs);
	if (result != AuthError_Success) return result;

	Serializer<GameData> s(ifs);
	try
	{
		s.Load(data);
		PLOG_VERBOSE << "게임 로드: Lv." << static_cast<int32_t>(data.CharacterData.Level) << " [" << data.CharacterData.Name << "]";
		return AuthError_Success;
	}
	catch (const std::ios::failure& fail)
	{
		PLOG_ERROR << "저장 파일을 읽어내지 못했습니다." << std::endl;
		PLOG_ERROR << fail.what() << std::endl;
		return AuthError_InternalServerError;
	}
}

int8_t	GameSession::SaveGame(const GameData& data)
{
	std::ifstream ifs;
	auto result = CheckAuth(_saveName, _savePassword, ifs);
	if (result != AuthError_Success && result != AuthError_NoAccount) return result;

	std::ofstream ofs(Hash(_saveName), std::ios::binary);
	if (!ofs) return AuthError_InternalServerError;
	ofs.exceptions(ofs.exceptions() | std::ios::failbit);

	Serializer<GameData> s(ofs);
	try
	{
		uint16_t len = static_cast<uint16_t>(_savePassword.length());
		ofs.write(reinterpret_cast<char*>(&len), sizeof(len));
		ofs.write(_savePassword.data(), _savePassword.length());
		s.Save(data);
		PLOG_VERBOSE << "게임 저장: Lv." << static_cast<int32_t>(data.CharacterData.Level) << " [" << data.CharacterData.Name << "]";
		ofs.flush(); 
		return AuthError_Success;
	}
	catch (const std::ios::failure& fail)
	{
		PLOG_ERROR << "Save error: " << fail.what();
		return AuthError_InternalServerError;
	}
}