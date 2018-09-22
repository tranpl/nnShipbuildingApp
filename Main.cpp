#define WIN32_LEAN_AND_MEAN // We don't want the extra stuff like MFC and such
#include <windows.h>
#include <XInput.h>     // XInput API
#pragma comment(lib, "XInput.lib")
#include<stdio.h>

int main() {
	DWORD dwResult;
	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));


	while (1) {
		// Get state of controller.
		dwResult = XInputGetState(0, &state);

		if (dwResult == ERROR_SUCCESS)
		{
			WORD bits = state.Gamepad.wButtons;

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
			else if (0x0100) {
				// left shoulder
				//printf("left shoulder");

			}

		}
		else
		{
			printf("Controller not connected");
		}

		//Sleep(250);
	}
	
	

	return 0;
}