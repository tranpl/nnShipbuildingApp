#define WIN32_LEAN_AND_MEAN // Dont include everything
#include <windows.h> // Input windows commands
#include <XInput.h>     // XInput API
#pragma comment(lib, "XInput.lib") // XInput setup
#include<stdio.h> // included for console IO

// method prototypes
void handleButtons();
void initController();

// global vars
SHORT prevL_X, prevL_Y, prevR_X, prevR_Y;
DWORD dwResult;
XINPUT_STATE state;
POINT mousePos;

int main() {
	// allocate memory
	ZeroMemory(&state, sizeof(XINPUT_STATE));

	// init positions
	initController();

	while (1) {
		// Get state of controller.
		dwResult = XInputGetState(0, &state);

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


	

	if (bits & 0x0001) {
		// then up dpad is pressed
		printf("up\n");
	}
	else if (bits & 0x0002) {
		// dpad down
		printf("down\n");
	}
	else if (bits & 0x0004) {
		// dpad left
		printf("left\n");
	}
	else if (bits & 0x0008) {
		// dpad right
		printf("right\n");
	}
	else if (bits & 0x0010) {
		// start
		printf("start\n");
	}
	else if (bits & 0x0020) {
		// back
		printf("back\n");
	}
	else if (bits & 0x0040) {
		// left joy stick
		printf("left joy");
	}
	else if (bits & 0x0080) {
		// right joy stick
		printf("right joy");
	}
	else if (bits & 0x0100) {
		// left shoulder
		printf("left shoulder");
	}
	else if (bits & 0x0200) {
		// right shoulder
		printf("right shoulder");
	}
	else if (bits & 0x1000) {
		// a button
		printf("a");
	}
	else if (bits & 0x2000) {
		// a button
		printf("b");
	}
	else if (bits & 0x4000) {
		// x button
		printf("x");


	}
	else if (bits & 0x8000) {
		// y button
		printf("y");
	}

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