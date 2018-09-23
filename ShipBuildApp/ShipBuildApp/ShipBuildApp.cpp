#define WIN32_LEAN_AND_MEAN // Dont include everything
#include <windows.h> // Input windows commands
#include <WinUser.h>
#include <XInput.h>     // XInput API
#pragma comment(lib, "XInput.lib") // XInput setup
#include<stdio.h> // included for console IO
#include<thread> // for threads

// Struct to hold if key is pressed
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
	bool START = false;
	bool BACK = false;
	bool rShoulder = false;
	bool lShoulder = false;
	bool rTrigger = false;
	bool lTrigger = false;
	bool lThumbPress = false;
	bool rThumbPress = false;

	bool B_TOGGLE = false;
	bool THREAD_JOIN = false;
};

// method prototypes
void handleButtons();
void initController();
void handleKeyCode(int keyStrokeValue);
void holdLeftClick();
void releaseLeftClick();
void handleJoyStick();

// global vars
SHORT prevL_X, prevL_Y, prevR_X, prevR_Y;
XINPUT_STATE state; // joy stick states
POINT mousePos; // mouse position
XINPUT_KEYSTROKE xinputkeystroke; // XInput struct


// struct that holds the pressed keys
KeysPressed keys;

int main() {
	// allocate memory
	ZeroMemory(&state, sizeof(XINPUT_STATE));

	DWORD reserved = { 0 };

	/*
	b button toggles bool
		true -> rotate cam
		
	in another thread
	*/

	std::thread joyStickThread(handleJoyStick);


	// init positions
	initController();

	while (1) {

		if (keys.THREAD_JOIN) {
			joyStickThread.join();
			printf("thread join");
		}

		// Get state of controller.
		DWORD dwResult = XInputGetState(0, &state);
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

		Sleep(5);
	}

	return 0;
}

void handleButtons() {

	WORD bits = state.Gamepad.wButtons;

	SHORT L_X = state.Gamepad.sThumbLX;
	SHORT L_Y = state.Gamepad.sThumbLY;

	// updates mouse left right
	if (L_X > 3000) {

		if ((GetCursorPos(&mousePos) != 0)) {
			prevL_X = state.Gamepad.sThumbLX;
			prevL_Y = state.Gamepad.sThumbLY;
		}
		
		printf("left stick moving\n");
		SetCursorPos(mousePos.x + 1, mousePos.y);
	}
	else if (L_X < -3000) {

		if ((GetCursorPos(&mousePos) != 0)) {
			prevL_X = state.Gamepad.sThumbLX;
			prevL_Y = state.Gamepad.sThumbLY;
		}

		printf("left stick moving\n");
		SetCursorPos(mousePos.x - 1, mousePos.y);
	}

	// updates mouse up down
	if (L_Y < -3000) {
		if ((GetCursorPos(&mousePos) != 0)) {
			prevL_X = state.Gamepad.sThumbLX;
			prevL_Y = state.Gamepad.sThumbLY;
		}

		printf("left stick moving\n");

		SetCursorPos(mousePos.x, mousePos.y + 1);
	}
	else if (L_Y > 3000) {
		if ((GetCursorPos(&mousePos) != 0)) {
			prevL_X = state.Gamepad.sThumbLX;
			prevL_Y = state.Gamepad.sThumbLY;
		}

		printf("left stick moving\n");

		SetCursorPos(mousePos.x, mousePos.y - 1);
	}



	unsigned short keyCode = xinputkeystroke.VirtualKey;

	if (keyCode != 0) {
		printf("Key Code: %d", keyCode);
		handleKeyCode(keyCode);
	}
}

// Steup up the default positions
void initController() {
	XInputGetState(0, &state);

	// set starting positions
	prevL_X = state.Gamepad.sThumbLX;
	prevL_Y = state.Gamepad.sThumbLY;
	prevR_X = state.Gamepad.sThumbRX;
	prevR_Y = state.Gamepad.sThumbRY;

}

/*
	Presses left or right click
*/
void MouseClick(int button) {
	printf("running MouseClick.");
	INPUT key = { 0 };
	key.type = INPUT_MOUSE;

	// left click
	if (button == VK_LBUTTON) {
		key.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
		SendInput(1, &key, sizeof(INPUT));
		key.mi.dwFlags = MOUSEEVENTF_LEFTUP;
		SendInput(1, &key, sizeof(INPUT));
	}
	// right click
	else if (button == VK_RBUTTON) {
		key.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
		SendInput(1, &key, sizeof(INPUT));
		key.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
		SendInput(1, &key, sizeof(INPUT));
	}
}

/*
	Method routes the game controller presses to the proper methods
*/
void handleKeyCode(int keyStrokeValue) {
	// x button
	switch (keyStrokeValue) {
	case VK_PAD_X:
		if (keys.X) {
			printf("X pressed");
			MouseClick(VK_LBUTTON);
		}

		if (xinputkeystroke.Flags == XINPUT_KEYSTROKE_KEYDOWN) {
			keys.X = true;
		}
		else {
			keys.X = false;
		}

		break;

	// y button
	case VK_PAD_Y:
		if (keys.Y) {
			printf("Y pressed");
			keys.THREAD_JOIN = true;
		}

		if (xinputkeystroke.Flags == XINPUT_KEYSTROKE_KEYDOWN) {
			keys.Y = true;
		}
		else {
			keys.Y = false;
		}
		break;

	case VK_PAD_A:
		if (keys.A) {
			printf("A BUtton Pressed.");
			MouseClick(VK_RBUTTON);
		}

		if (xinputkeystroke.Flags == XINPUT_KEYSTROKE_KEYDOWN) {
			keys.A = true;
		}
		else {
			keys.A = false;
		}
		break;
	case VK_PAD_B:
		
		if (keys.B) {
			printf("B BUtton Pressed.");
			keys.B_TOGGLE = !keys.B_TOGGLE;
		}

		if (xinputkeystroke.Flags == XINPUT_KEYSTROKE_KEYDOWN) {
			keys.B = true;
		}
		else {
			keys.B = false;
		}
		break;
	case VK_PAD_LTRIGGER:
		if (keys.B) {
			printf("Left Trigger BUtton Pressed.");
		}

		if (xinputkeystroke.Flags == XINPUT_KEYSTROKE_KEYDOWN) {
			keys.B = true;
		}
		else {
			keys.B = false;
		}
		break;
	case VK_PAD_RTRIGGER:
		if (keys.B) {
			printf("Right Trigger BUtton Pressed.");
		}

		if (xinputkeystroke.Flags == XINPUT_KEYSTROKE_KEYDOWN) {
			keys.B = true;
		}
		else {
			keys.B = false;
		}
		break;
	case VK_PAD_LSHOULDER:
		if (keys.lShoulder) {
			printf("Left Shoulder BUtton Pressed.");
		}

		if (xinputkeystroke.Flags == XINPUT_KEYSTROKE_KEYDOWN) {
			keys.lShoulder = true;
		}
		else {
			keys.lShoulder = false;
		}
		break;
	case VK_PAD_RSHOULDER:
		if (keys.rShoulder) {
			printf("Right Shoulder BUtton Pressed.");
		}

		if (xinputkeystroke.Flags == XINPUT_KEYSTROKE_KEYDOWN) {
			keys.rShoulder = true;
		}
		else {
			keys.rShoulder = false;
		}
		break;
	case VK_PAD_START:
		if (keys.START) {
			printf("Start BUtton Pressed.");
		}

		if (xinputkeystroke.Flags == XINPUT_KEYSTROKE_KEYDOWN) {
			keys.START = true;
		}
		else {
			keys.START = false;
		}
		break;
	case VK_PAD_BACK:
		if (keys.BACK) {
			printf("Back BUtton Pressed.");
		}

		if (xinputkeystroke.Flags == XINPUT_KEYSTROKE_KEYDOWN) {
			keys.BACK = true;
		}
		else {
			keys.BACK = false;
		}
		break;
	case VK_PAD_LTHUMB_PRESS:
		if (keys.lThumbPress) {
			printf("Left Thumb BUtton Pressed.");
		}

		if (xinputkeystroke.Flags == XINPUT_KEYSTROKE_KEYDOWN) {
			keys.lThumbPress = true;
		}
		else {
			keys.lThumbPress = false;
		}
		break;
	case VK_PAD_RTHUMB_PRESS:
		if (keys.rThumbPress) {
			printf("Right Thumb BUtton Pressed.");
		}

		if (xinputkeystroke.Flags == XINPUT_KEYSTROKE_KEYDOWN) {
			keys.rThumbPress = true;
		}
		else {
			keys.rThumbPress = false;
		}
		break;
	case VK_PAD_DPAD_RIGHT:
		if (keys.rThumbPress) {
			printf("D PAD RIGHT BUtton Pressed.");
		}

		if (xinputkeystroke.Flags == XINPUT_KEYSTROKE_KEYDOWN) {
			keys.rThumbPress = true;
		}
		else {
			keys.rThumbPress = false;
		}
		break;
	case VK_PAD_DPAD_LEFT:
		if (keys.rThumbPress) {
			printf("D PAD LEFT BUtton Pressed.");
		}

		if (xinputkeystroke.Flags == XINPUT_KEYSTROKE_KEYDOWN) {
			keys.rThumbPress = true;
		}
		else {
			keys.rThumbPress = false;
		}
		break;
	case VK_PAD_DPAD_DOWN:
		if (keys.rThumbPress) {
			printf("D PAD DOWN BUtton Pressed.");
		}

		if (xinputkeystroke.Flags == XINPUT_KEYSTROKE_KEYDOWN) {
			keys.rThumbPress = true;
		}
		else {
			keys.rThumbPress = false;
		}
		break;
	case VK_PAD_DPAD_UP:
		if (keys.rThumbPress) {
			printf("D PAD UP Button Pressed.");
		}

		if (xinputkeystroke.Flags == XINPUT_KEYSTROKE_KEYDOWN) {
			keys.rThumbPress = true;
		}
		else {
			keys.rThumbPress = false;
		}
		break;
	default:
		releaseLeftClick();
		break;
	}

}

void holdLeftClick() {
	INPUT key = { 0 };
	key.type = INPUT_MOUSE;
	key.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	SendInput(1, &key, sizeof(INPUT));
}


void releaseLeftClick() {
	INPUT key = { 0 };
	key.type = INPUT_MOUSE;
	key.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &key, sizeof(INPUT));
}

void handleJoyStick() {
	/*

	below is right stick

	*/
	// updates mouse left right
	SHORT R_X = state.Gamepad.sThumbRX;
	SHORT R_Y = state.Gamepad.sThumbRY;
	printf("thread started\n");
	
	if (R_X > 3000) {

		if ((GetCursorPos(&mousePos) != 0)) {
			prevR_X = state.Gamepad.sThumbRX;
			prevR_Y = state.Gamepad.sThumbRY;
		}

		holdLeftClick();
		SetCursorPos(mousePos.x + 1, mousePos.y);
	}
	else if (R_X < -3000) {

		if ((GetCursorPos(&mousePos) != 0)) {
			prevR_X = state.Gamepad.sThumbRX;
			prevR_Y = state.Gamepad.sThumbRY;
		}

		holdLeftClick();
		SetCursorPos(mousePos.x - 1, mousePos.y);
	}
	else {
		releaseLeftClick();
	}


	// updates mouse up down
	if (R_Y < -3000) {
		if ((GetCursorPos(&mousePos) != 0)) {
			prevL_X = state.Gamepad.sThumbLX;
			prevL_Y = state.Gamepad.sThumbLY;
		}

		holdLeftClick();

		SetCursorPos(mousePos.x, mousePos.y + 1);
	}
	else if (R_Y > 3000) {
		if ((GetCursorPos(&mousePos) != 0)) {
			prevL_X = state.Gamepad.sThumbLX;
			prevL_Y = state.Gamepad.sThumbLY;
		}

		holdLeftClick();

		SetCursorPos(mousePos.x, mousePos.y - 1);
	}
	else {
		releaseLeftClick();
	}

	
}