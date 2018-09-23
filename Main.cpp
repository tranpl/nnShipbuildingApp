#define WIN32_LEAN_AND_MEAN // We don't want the extra stuff like MFC and such
#include <windows.h>
#include <WinUser.h>
#include <XInput.h>     // XInput API
#pragma comment(lib, "XInput.lib")
#include<stdio.h>

void click(int button) {
	INPUT key;
	key.type = INPUT_MOUSE;
	key.mi.dwFlags = button;
	SendInput(1, &key, sizeof(INPUT));
	key.mi.dwFlags = button;
	SendInput(1, &key, sizeof(INPUT));
}

void press(int vkKeyCode) {
	INPUT key;
	key.type = INPUT_KEYBOARD;
	key.ki.wVk = vkKeyCode;
	SendInput(1, &key, sizeof(INPUT));
}
