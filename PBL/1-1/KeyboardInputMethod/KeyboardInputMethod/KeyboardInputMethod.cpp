#include <Windows.h>
#include "KeyboardInputMethod.hpp"

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR w, _In_ int w2)
{
	//TestWindowMessage(hInstance, hPrevInstance, w, w2);
	//TestGetKeyState(hInstance, hPrevInstance, w, w2);
	TestGetAsyncKeyState(hInstance, hPrevInstance, w, w2);
	return 0;
}