#pragma once

#include <cstdint>

constexpr int32_t PORT = 4242;
constexpr int32_t MAX_CLIENTS = 2000;

enum ServerToClient {
	ServerToClient_LoadRes,
	ServerToClient_SaveAuthRes,
	ServerToClient_SaveRes
};

enum ClientToServer {
	ClientToServer_LoadReq,
	ClientToServer_SaveAuthReq,
	ClientToServer_SaveReq
};

enum AuthError
{
	AuthError_Success,             // 오류가 없는 성공
	AuthError_EmptyValue,          // 아이디 또는 패스워드가 비어있음
	AuthError_InvalidPassword,     // 로그인 실패 (계정은 존재하나, 패스워드 틀림)
	AuthError_NoAccount,           // 해당 계정으로 아이디 없음
	AuthError_InternalServerError  // 파일 입출력 관련 에러 발생
};