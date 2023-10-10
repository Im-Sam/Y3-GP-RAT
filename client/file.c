#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "192.168.0.20"
#define SERVER_PORT 9030
#define BUFFER_SIZE 1024

int main()
{
    WSADATA wsaData;
    SOCKET clientSocket;
    struct sockaddr_in serverAddress;
    FILE *file;
    char filePath[MAX_PATH];
    char buffer[BUFFER_SIZE];
    int bytesRead, bytesSent;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("Failed to initialize Winsock.\n");
        return 1;
    }

    // Create client socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET)
    {
        printf("Failed to create socket: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Set up server address
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVER_PORT);
    serverAddress.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        printf("Failed to connect to server.\n");
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // Get absolute file path
    printf("Enter absolute file path: ");
    fgets(filePath, sizeof(filePath), stdin);
    filePath[strlen(filePath) - 1] = '\0'; // Remove newline character

    // Open the file for reading
    file = fopen(filePath, "rb");
    if (file == NULL)
    {
        printf("Failed to open file.\n");
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // Read and send file data
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0)
    {
        bytesSent = 0;
        while (bytesSent < bytesRead)
        {
            int sent = send(clientSocket, buffer + bytesSent, bytesRead - bytesSent, 0);
            if (sent <= 0)
            {
                printf("Failed to send data.\n");
                fclose(file);
                closesocket(clientSocket);
                WSACleanup();
                return 1;
            }
            bytesSent += sent;
        }
    }

    printf("File sent successfully.\n");

    // Cleanup
    fclose(file);
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
