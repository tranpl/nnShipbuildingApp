#define WIN32_LEAN_AND_MEAN // Dont include everything
#include <windows.h> // Input windows commands
#include <XInput.h>     // XInput API
#pragma comment(lib, "XInput.lib") // XInput setup
#include<stdio.h> // included for console IO
#include <iostream>
struct KeysPressed
{
	bool X = false;
	bool Y = false;
	bool A = false;
	bool B = false;
	bool dUP = false;
	bool dDown = false;
	bool dLeft = false;
	bool dRight = false;
};

// method prototypes
void handleButtons();
void initController();
void GamePadIsPressed(int keyStrokeValue);

// global vars
SHORT prevL_X, prevL_Y, prevR_X, prevR_Y;
DWORD dwResult;
XINPUT_STATE state;
POINT mousePos;
DWORD reserved;
XINPUT_KEYSTROKE xinputkeystroke;

KeysPressed keys;

int main() {
	// allocate memory
	ZeroMemory(&state, sizeof(XINPUT_STATE));

	// init positions
	initController();

	while (1) {
		// Get state of controller.
		dwResult = XInputGetState(0, &state);
		XInputGetKeystroke(0, reserved, &xinputkeystroke);
		if (dwResult == ERROR_SUCCESS)
		{
			handleButtons();
		}
		else
		{
			printf("Controller not connected");
			// handle disconnect later
			break;
		}

		Sleep(1);
	}

	return 0;
}

void handleButtons() {

	WORD bits = state.Gamepad.wButtons;

	SHORT L_X = state.Gamepad.sThumbLX;
	SHORT L_Y = state.Gamepad.sThumbLY;
	SHORT R_X = state.Gamepad.sThumbRX;
	SHORT R_Y = state.Gamepad.sThumbRY;

	// updates mouse left right
	if (L_X > 3000) {

		if ((GetCursorPos(&mousePos) != 0)) {
			prevL_X = state.Gamepad.sThumbLX;
			prevL_Y = state.Gamepad.sThumbLY;
		}

		SetCursorPos(mousePos.x + 1, mousePos.y);
	}
	else if (L_X < -3000) {

		if ((GetCursorPos(&mousePos) != 0)) {
			prevL_X = state.Gamepad.sThumbLX;
			prevL_Y = state.Gamepad.sThumbLY;
		}

		SetCursorPos(mousePos.x - 1, mousePos.y);
	}

	// updates mouse up down
	if (L_Y < -3000) {
		if ((GetCursorPos(&mousePos) != 0)) {
			prevL_X = state.Gamepad.sThumbLX;
			prevL_Y = state.Gamepad.sThumbLY;
		}

		SetCursorPos(mousePos.x, mousePos.y + 1);
	}
	else if (L_Y > 3000) {
		if ((GetCursorPos(&mousePos) != 0)) {
			prevL_X = state.Gamepad.sThumbLX;
			prevL_Y = state.Gamepad.sThumbLY;
		}

		SetCursorPos(mousePos.x, mousePos.y - 1);
	}

	/*

	all button presses below


	*/

	unsigned short KeyCode = xinputkeystroke.VirtualKey;
	
	if (KeyCode != 0) {
		printf("%d\n", KeyCode);
		GamePadIsPressed(KeyCode);
	}
	//if (bits & 0x0001) {
	//	// then up dpad is pressed
	//	printf("up\n");
	//}
	//else if (bits & 0x0002) {
	//	// dpad down
	//	printf("down\n");
	//}
	//else if (bits & 0x0004) {
	//	// dpad left
	//	printf("left\n");
	//}
	//else if (bits & 0x0008) {
	//	// dpad right
	//	printf("right\n");
	//}
	//else if (bits & 0x0010) {
	//	// start
	//	printf("start\n");
	//}
	//else if (bits & 0x0020) {
	//	// back
	//	printf("back\n");
	//}
	//else if (bits & 0x0040) {
	//	// left joy stick
	//	printf("left joy");
	//}
	//else if (bits & 0x0080) {
	//	// right joy stick
	//	printf("right joy");
	//}
	//else if (bits & 0x0100) {
	//	// left shoulder
	//	printf("left shoulder");
	//}
	//else if (bits & 0x0200) {
	//	// right shoulder
	//	printf("right shoulder");
	//}
	//else if (bits & 0x1000) {
	//	// a button
	//	printf("a");
	//}
	//else if (bits & 0x2000) {
	//	// a button
	//	printf("b");
	//}
	////else if (bits & 0x4000) {
	//else if(xinputkeystroke.VirtualKey == 22530){

	//	//GamePadIsPressed();
	//}
	//else if (bits & 0x8000) {
	//	// y button
	//	printf("y");
	//}

}

// return true if setup works
void initController() {
	dwResult = XInputGetState(0, &state);

	// set starting positions
	prevL_X = state.Gamepad.sThumbLX;
	prevL_Y = state.Gamepad.sThumbLY;
	prevR_X = state.Gamepad.sThumbRX;
	prevR_Y = state.Gamepad.sThumbRY;

}
void MouseClick(int button) {
	INPUT key;
	key.type = INPUT_MOUSE;
	key.mi.dwFlags = button;
	SendInput(1, &key, sizeof(INPUT));
	key.mi.dwFlags = button;
	SendInput(1, &key, sizeof(INPUT));
}

void KeyPress(int vkKeyCode) {
	INPUT key;
	key.type = INPUT_KEYBOARD;
	key.ki.wVk = vkKeyCode;
	SendInput(1, &key, sizeof(INPUT));
}

void GamePadIsPressed(int keyStrokeValue) {
	// x button
	switch (keyStrokeValue) {
		case VK_PAD_X:
			/*
				Press X button
			*/
			MouseClick(VK_LBUTTON);
			break;
		case VK_PAD_Y:
			break;
		case VK_PAD_A:
			break;
		case VK_PAD_B:
			break;

	}
	if (keyStrokeValue == VK_PAD_X) {
		if (keys.X) {
			printf("X BUtton Pressed.");
		}

		if (xinputkeystroke.Flags == XINPUT_KEYSTROKE_KEYDOWN) {
			keys.X = true;
		}
		else {
			keys.X = false;
		}
	}
}