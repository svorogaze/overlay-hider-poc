#include <Windows.h>
#include <stdio.h>
struct mydata {
    HWND ptr;
};


int main() {
    HANDLE hdevice = CreateFile(L"\\\\.\\pogk", GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);
    if (hdevice == INVALID_HANDLE_VALUE) {
        printf("meow");
    }
    else {
        HWND h = FindWindow(NULL, L"aboba");
        if (h == INVALID_HANDLE_VALUE) {
            printf("cry about it");
        }
        else {
            printf("%d\n", IsWindowVisible(h));
            mydata d;
            DWORD bytes;
            DeviceIoControl(hdevice, 0x777, &h, sizeof(mydata), nullptr, 0, &bytes, nullptr);
            Sleep(5000);
            printf("%d\n", IsWindowVisible(h));
            RECT mw;
            GetWindowRect(h, &mw);
            printf("%d %d\n", mw.right - mw.left, mw.bottom - mw.top);
        }
    }
    Sleep(500000);
}

