#define WIN32_LEAN_AND_MEAN // Dont include everything
#include <windows.h> // Input windows commands
#include <WinUser.h>
#include <XInput.h>     // XInput API
#pragma comment(lib, "XInput.lib") // XInput setup
#include<stdio.h> // included for console IO

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
	bool Y_TOGGLE = false;

	// left stick 
	bool down = false;
	bool up = false;
	bool right = false;
	bool left = false;

};

// method prototypes
void handleButtons();
void initController();
void handleKeyCode(int keyStrokeValue);
void handleLeftStick();


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

	// init positions
	initController();

	while (1) {

		// Get state of controller.
		DWORD dwResult = XInputGetState(0, &state);
		XInputGetKeystroke(0, reserved, &xinputkeystroke);
		if (dwResult == ERROR_SUCCESS)
		{
			handleButtons();
			handleLeftStick();
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

	case VK_PAD_START:
		keys.B_TOGGLE = false;
		keys.Y_TOGGLE = false;
	break;

	case VK_PAD_Y:


		if (keys.B_TOGGLE) {
			// prevents directX bug in the Sap software
			return;
		}

		keys.Y_TOGGLE = !keys.Y_TOGGLE;
		
		if (keys.Y_TOGGLE) {
			INPUT key;
			key.type = INPUT_MOUSE;
			key.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
			SendInput(1, &key, sizeof(INPUT));
		}
		else {
			INPUT key;
			key.type = INPUT_MOUSE;
			key.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
			SendInput(1, &key, sizeof(INPUT));
		}

		break;

	case VK_PAD_B:

		printf("B BUtton Pressed.");

		keys.B_TOGGLE = !keys.B_TOGGLE;

		if (keys.B_TOGGLE) {
			INPUT key;
			key.type = INPUT_MOUSE;
			key.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
			SendInput(1, &key, sizeof(INPUT));
		}
		else {
			INPUT key;
			key.type = INPUT_MOUSE;
			key.mi.dwFlags = MOUSEEVENTF_LEFTUP;
			SendInput(1, &key, sizeof(INPUT));
		}

		break;

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
		if (keys.rTrigger) {
			INPUT key = { 0 };
			key.type = MOUSE_EVENT;
			key.mi.dwFlags = VK_RBUTTON;
			SendInput(1, &key, sizeof(INPUT));
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

			INPUT key = { 0 };
			key.type = INPUT_KEYBOARD;
			key.ki.wVk = VK_PRIOR;
			SendInput(1, &key, sizeof(INPUT));
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

			INPUT key = { 0 };
			key.type = INPUT_KEYBOARD;
			key.ki.wVk = VK_NEXT;
			SendInput(1, &key, sizeof(INPUT));
		}

		if (xinputkeystroke.Flags == XINPUT_KEYSTROKE_KEYDOWN) {
			keys.rShoulder = true;
		}
		else {
			keys.rShoulder = false;
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
		break;
	}

}

void handleLeftStick() {

	SHORT L_X = state.Gamepad.sThumbLX;
	SHORT L_Y = state.Gamepad.sThumbLY;

	if (L_X > 3000) {
		keys.right = true;
		keys.left = false;
	}
	else if (L_X < -3000) {
		keys.left = true;
		keys.right = false;
	}
	else {
		keys.right = false;
		keys.left = false;
	}

	if (L_Y > 3000) {
		keys.up = true;
		keys.down = false;
	}
	else if (L_Y < -3000) {
		keys.down = true;
		keys.up = false;
	}
	else {
		keys.up = false;
		keys.down = false;
	}

	if (keys.down) {
		if ((GetCursorPos(&mousePos) != 0)) {
			SetCursorPos(mousePos.x, mousePos.y + 1);
		}

	}
	else if (keys.up) {
		if ((GetCursorPos(&mousePos) != 0)) {
			SetCursorPos(mousePos.x, mousePos.y - 1);
		}
	}

	if (keys.right) {
		if ((GetCursorPos(&mousePos) != 0)) {
			SetCursorPos(mousePos.x + 1, mousePos.y);
		}
	}
	else if (keys.left) {
		if ((GetCursorPos(&mousePos) != 0)) {
			SetCursorPos(mousePos.x - 1, mousePos.y);
		}
	}
}