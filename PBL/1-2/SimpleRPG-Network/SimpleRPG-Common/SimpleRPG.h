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
	AuthError_Success,             // ������ ���� ����
	AuthError_EmptyValue,          // ���̵� �Ǵ� �н����尡 �������
	AuthError_InvalidPassword,     // �α��� ���� (������ �����ϳ�, �н����� Ʋ��)
	AuthError_NoAccount,           // �ش� �������� ���̵� ����
	AuthError_InternalServerError  // ���� ����� ���� ���� �߻�
};