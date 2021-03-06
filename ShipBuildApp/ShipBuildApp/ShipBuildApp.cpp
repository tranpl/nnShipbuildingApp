/*
	Authors:
	Matt Mawyer
	Michael Pigeon
	Michael Tran
	Melanie Ung

	Virginia Commmonwealth University RAMHACK 2018
	September 23, 2018
	----------------------------------------------

	ShipBuildApp.cpp:

	-Main class desgined to interface with Sap 3D Viewer
	-Takes an xbox 360 controller as input for the program

	-Make sure there is an xbox 360 controller plugged in, else program will terminate <-------

*/

#define WIN32_LEAN_AND_MEAN // Dont include everything
#include <windows.h> // Input windows commands
#include <WinUser.h>
#include <XInput.h>     // XInput API
#pragma comment(lib, "XInput.lib") // XInput setup
#include<stdio.h> // included for console IO
#include<string.h>
#include<string>

// Struct to hold if key is pressed
struct KeysPressed
{
	// X button
	bool X = false;

	// Y button
	bool Y = false;

	// A button
	bool A = false;

	// B button
	bool B = false;

	// Dpad up
	bool dUP = false;

	// Dpad down
	bool dDown = false;

	// Dpad left
	bool dLeft = false;

	// Dpad right
	bool dRight = false;

	// Start button
	bool START = false;

	// used to toggle B button
	bool B_TOGGLE = false;

	// used to toggle Y button
	bool Y_TOGGLE = false;

	// left stick direction
	bool down = false;
	bool up = false;
	bool right = false;
	bool left = false;

	/*
	vars for buttons that were not used, but were planned to be used
	*/
	bool BACK = false;
	bool rShoulder = false;
	bool lShoulder = false;
	bool rTrigger = false;
	bool lTrigger = false;
	bool lThumbPress = false;
	bool rThumbPress = false;
};

/*

Method prototypes

*/

// handles button presses
void handleButtons();

// setups on gamepad
void initController();

// handles individual keys
void handleKeyCode(int keyStrokeValue);

// handles joystick input
void handleLeftStick();

// creates clipboard
void toClipboard(HWND hwnd, const std::string &s);

// pastes from the clipboard
void pasteClipboard();

/*

Global vars

*/

// vars for joystick
SHORT prevL_X, prevL_Y, prevR_X, prevR_Y;

// state struct for gamepad
XINPUT_STATE state;

// moues position
POINT mousePos;

// struct for keypresses
XINPUT_KEYSTROKE xinputkeystroke;


// struct that holds the pressed keys
KeysPressed keys;

int main() {
	// allocate memory
	ZeroMemory(&state, sizeof(XINPUT_STATE));

	// reserved for GetKeystroke
	DWORD reserved = { 0 };

	// setup the gamepad
	initController();

	// loop -> poll keyboard
	while (1) {

		// Poll the game pad for inputs
		DWORD dwResult = XInputGetState(0, &state);

		// Store the pressed keys in the xinputkeystoke struct
		XInputGetKeystroke(0, reserved, &xinputkeystroke);

		// check for error on polling
		if (dwResult == ERROR_SUCCESS)
		{
			handleButtons();
			handleLeftStick();
		}
		else
		{

			//printf("Controller not connected");
			
			/*
				Handle gamepad diconnections here
					Wait for gamepad connect, or close program

				currently will close program (for debugging)
			*/

			break;
		}

		// used for debugging, and to slow down input polling
		Sleep(5);
	}

	return 0;
}

/*
Assigns the gamepad button press to the right method
Mostly used as a wrapper to filter the input
*/
void handleButtons() {

	unsigned short keyCode = xinputkeystroke.VirtualKey;

	if (keyCode != 0) {
		//debug line
		//printf("Key Code: %d", keyCode);
		handleKeyCode(keyCode);
	}
}

/*
Initializes the starting cordinates of the controller
*/
void initController() {
	// Setup the gamepad
	XInputGetState(0, &state);

	// set starting positions
	prevL_X = state.Gamepad.sThumbLX;
	prevL_Y = state.Gamepad.sThumbLY;
	prevR_X = state.Gamepad.sThumbRX;
	prevR_Y = state.Gamepad.sThumbRY;

}

/*
	Press and release left or right click
*/
void MouseClick(int button) {
	// debug line
	//printf("running MouseClick.");
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
	Handles all button events from the game pad
*/
void handleKeyCode(int keyStrokeValue) {
	
	
	switch (keyStrokeValue) {

		// clear toggles
	case VK_PAD_START:
		keys.B_TOGGLE = false;
		keys.Y_TOGGLE = false;
	break;

		// y button
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

		// b button
	case VK_PAD_B:

		// debug line
		//printf("B BUtton Pressed.");

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

		// x button
	case VK_PAD_X:
		if (keys.X) {
			printf("X pressed");
			MouseClick(VK_LBUTTON);
		}

		/*
			The following if-else will be seen throughout the code,
			since the program is polling for gamepad input very quickly
			it will detect that button is being held down even though
			the button was pressed very quickly
		*/

		if (xinputkeystroke.Flags == XINPUT_KEYSTROKE_KEYDOWN) {
			keys.X = true;
		}
		else {
			keys.X = false;
		}
		break;

		// a button
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
	/*
	didn't implement

	case VK_PAD_LSHOULDER:
		if (keys.lShoulder) {

			std::string str;
			HWND hwnd = GetDesktopWindow();
			hwnd = GetWindow(hwnd, 0);
			toClipboard(hwnd, str);

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

			INPUT key[2];
			key[0].type = INPUT_KEYBOARD;
			key[0].ki.wVk = VK_CONTROL;
			key[1].type = INPUT_KEYBOARD;
			key[1].ki.wVk = 0x56;
			SendInput(2, key, sizeof(INPUT));
		}

		if (xinputkeystroke.Flags == XINPUT_KEYSTROKE_KEYDOWN) {
			keys.rShoulder = true;
		}
		else {
			keys.rShoulder = false;
		}
		break;
		*/
	case VK_PAD_DPAD_DOWN:
		if (keys.dUP) {
			INPUT key = { 0 };
			key.type = INPUT_KEYBOARD;
			key.ki.wVk = VK_PRIOR;
			SendInput(1, &key, sizeof(INPUT));
		}

		if (xinputkeystroke.Flags == XINPUT_KEYSTROKE_KEYDOWN) {
			keys.dUP = true;
		}
		else {
			keys.dUP = false;
		}
		break;
	case VK_PAD_DPAD_UP:
		if (keys.rThumbPress) {
			INPUT key = { 0 };
			key.type = INPUT_KEYBOARD;
			key.ki.wVk = VK_NEXT;
			SendInput(1, &key, sizeof(INPUT));
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

/*
Handles the actions of the joy stick
*/
void handleLeftStick() {

	SHORT L_X = state.Gamepad.sThumbLX;
	SHORT L_Y = state.Gamepad.sThumbLY;

	// Set axis of rotation
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

	// Adjust mouse position based on joystick

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

/*
Create clipboard method, didnt implement fully
Wouldn't copy from SAP Software
*/
void toClipboard(HWND hwnd, const std::string &s) {
	OpenClipboard(hwnd);
	//EmptyClipboard();
	HGLOBAL heap = GlobalAlloc(GMEM_MOVEABLE, s.size() + 1);
	if (!heap) {
		CloseClipboard();
		return;
	}
	memcpy(GlobalLock(heap), s.c_str(), s.size() + 1);
	GlobalUnlock(heap);
	SetClipboardData(CF_TEXT, heap);
	CloseClipboard();
	GlobalFree(heap);
}

/*
Paste method, didn't implement
*/
void pasteClipboard() {

}