#include <Windows.h>
#include <iostream>
#include <string>
#include <conio.h>
#include "KeyboardInputMethod.hpp"

static LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
static bool g_key[0x100];

constexpr auto g_size = SIZE{ 100, 100 };
constexpr auto g_programName = "KeyboardInputMethod";

void TestGetKeyState(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
	WNDCLASSEX wc;
	MSG msg;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	setlocale(LC_ALL, "");
	_wsetlocale(LC_ALL, L"");
	if (AllocConsole())
	{
		FILE* out1, * out2, * out3;
		freopen_s(&out1, "CONIN$", "rb", stdin);
		freopen_s(&out2, "CONOUT$", "wb", stdout);
		freopen_s(&out3, "CONOUT$", "wb", stderr);
		std::ios::sync_with_stdio();
	}

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = g_programName;

	RECT wr = { 0, 0, g_size.cx, g_size.cy };
	AdjustWindowRect(&wr, WS_CAPTION | WS_SYSMENU, FALSE);

	RegisterClassEx(&wc);
	HWND hWnd = CreateWindowEx(NULL,
		g_programName,
		g_programName,
		WS_CAPTION | WS_SYSMENU,
		100, 100,
		wr.right - wr.left, wr.bottom - wr.top,
		NULL, NULL, wc.hInstance, NULL);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			const auto down = GetKeyState(VK_DOWN);
			const auto up = GetKeyState(VK_UP);
			const auto left = GetKeyState(VK_LEFT);
			const auto right = GetKeyState(VK_RIGHT);
			const auto shift = GetKeyState(VK_SHIFT);
			const auto control = GetKeyState(VK_CONTROL);
			const auto alt = GetKeyState(VK_MENU);

			auto IsPressed = [](SHORT keyState) { return (keyState & 0x8000) != 0; };
			g_key[VK_DOWN] = IsPressed(down);
			g_key[VK_UP] = IsPressed(up);
			g_key[VK_LEFT] = IsPressed(left);
			g_key[VK_RIGHT] = IsPressed(right);
			g_key[VK_SHIFT] = IsPressed(shift);
			g_key[VK_CONTROL] = IsPressed(control);
			g_key[VK_MENU] = IsPressed(alt);

			Sleep(2000);

			std::printf("Shift: %1d  |  Ctrl: %1d  |  Alt: %1d  |  ??: %1d  |  ??: %1d  |  ??: %1d  |  ??: %1d\n",
				g_key[VK_SHIFT], g_key[VK_CONTROL], g_key[VK_MENU], g_key[VK_LEFT], g_key[VK_RIGHT], g_key[VK_UP], g_key[VK_DOWN]);

		}
	}
}

union KeyState
{
	LPARAM lparam;

	struct
	{
		unsigned nRepeatCount : 16;
		unsigned nScanCode : 8;
		unsigned nExtended : 1;
		unsigned nReserved : 4;
		unsigned nContext : 1;
		unsigned nPrev : 1;
		unsigned nTrans : 1;
	};
};

static LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}