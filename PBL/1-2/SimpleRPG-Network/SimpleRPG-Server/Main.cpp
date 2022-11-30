#include "pch.h"
#include <SimpleRPG.h>
#include "GameSession.h"

GameSession	g_clients[MAX_CLIENTS];
std::mutex	g_mutex;
uv_loop_t	g_loop;

static void		OnAccept(uv_stream_t* server, int status);
static int32_t	FindEmptySlot();
static void		DropSession(uv_stream_t* server);

int main()
{
	static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
	plog::init(plog::verbose, &consoleAppender);

	uv_loop_init(&g_loop);

	uv_tcp_t	server;
	uv_tcp_init(&g_loop, &server);

	sockaddr_in	addr{};
	uv_ip4_addr("0.0.0.0", PORT, &addr);

	uv_tcp_bind(&server, reinterpret_cast<const sockaddr*>(&addr), 0);
	auto result = uv_listen(reinterpret_cast<uv_stream_t*>(&server), 10, OnAccept);
	if (result)
	{
		PLOG_ERROR << "Listen error: " << uv_strerror(result);
		return EXIT_FAILURE;
	}
	PLOG_INFO << "Listening on port " << PORT;

	return uv_run(&g_loop, UV_RUN_DEFAULT);
}

void OnAccept(uv_stream_t* server, int status)
{
	if (status < 0)
	{
		PLOG_ERROR << "Accept failed: " << uv_strerror(status);
		return;
	}

	const auto slot = FindEmptySlot();
	if (slot == -1)
	{
		PLOG_ERROR << "Accept failed: exceed maximum clients";
		DropSession(server);
		return;
	}
	
	const auto result = uv_accept(server, reinterpret_cast<uv_stream_t*>(g_clients[slot].GetHandle()));
	if (result)
	{
		PLOG_ERROR << "Accept failed: " << uv_strerror(status);
		g_clients[slot].Close();
		return;
	}
	PLOG_INFO << "Accept new connection [" << slot << "]";

	uv_read_start(
		reinterpret_cast<uv_stream_t*>(g_clients[slot].GetHandle()), 
		AbstractSession::AllocBuffer, 
		AbstractSession::OnRead);
}

int32_t FindEmptySlot()
{
	int32_t slot = -1;
	std::lock_guard<std::mutex> guard(g_mutex);
	for (int32_t i = 0; i < std::ssize(g_clients); i++)
	{
		if (g_clients[i].GetSocketId() > 0) continue;
		new(&g_clients[slot = i]) GameSession(&g_loop);
		break;
	}
	return slot;
}

void DropSession(uv_stream_t* server)
{
	uv_tcp_t client;
	uv_tcp_init(&g_loop, &client);
	uv_accept(server, reinterpret_cast<uv_stream_t*>(&client));
	uv_close(reinterpret_cast<uv_handle_t*>(&client), nullptr);
}