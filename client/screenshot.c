#include <stdio.h>
#include <winsock2.h>
#include <windows.h>

#define SERVER_ADDRESS "192.168.0.20"
#define SERVER_PORT 9040

int main() {
    // Get the screen dimensions
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);

    // Create a device context for the entire screen
    HDC screenDC = GetDC(NULL);
    if (screenDC == NULL) {
        printf("Failed to get screen device context\n");
        return 1;
    }

    // Create a compatible device context and bitmap
    HDC memDC = CreateCompatibleDC(screenDC);
    HBITMAP bitmap = CreateCompatibleBitmap(screenDC, width, height);
    if (bitmap == NULL) {
        printf("Failed to create bitmap\n");
        ReleaseDC(NULL, screenDC);
        return 1;
    }

    // Select the bitmap into the device context
    HBITMAP oldBitmap = SelectObject(memDC, bitmap);

    // Copy the screen contents into the bitmap
    BitBlt(memDC, 0, 0, width, height, screenDC, 0, 0, SRCCOPY);

    // Save the bitmap as a BMP file
    BITMAPINFOHEADER bi;
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = width;
    bi.biHeight = -height;
    bi.biPlanes = 1;
    bi.biBitCount = 24;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    BITMAPFILEHEADER bf;
    bf.bfType = 0x4D42;
    bf.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + width * height * 3;
    bf.bfReserved1 = 0;
    bf.bfReserved2 = 0;
    bf.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // Retrieve bitmap data
    BYTE* bitmapBits = (BYTE*)malloc(width * height * 3);
    if (bitmapBits == NULL) {
        printf("Failed to allocate memory for bitmap data\n");
        SelectObject(memDC, oldBitmap);
        DeleteObject(bitmap);
        DeleteDC(memDC);
        ReleaseDC(NULL, screenDC);
        return 1;
    }

    if (!GetDIBits(memDC, bitmap, 0, height, bitmapBits, (BITMAPINFO*)&bi, DIB_RGB_COLORS)) {
        printf("Failed to retrieve bitmap data\n");
        free(bitmapBits);
        SelectObject(memDC, oldBitmap);
        DeleteObject(bitmap);
        DeleteDC(memDC);
        ReleaseDC(NULL, screenDC);
        return 1;
    }

    FILE* file = fopen("screenshot.bmp", "wb");
    if (file == NULL) {
        printf("Failed to open file\n");
        free(bitmapBits);
        SelectObject(memDC, oldBitmap);
        DeleteObject(bitmap);
        DeleteDC(memDC);
        ReleaseDC(NULL, screenDC);
        return 1;
    }

    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, file);
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, file);
    fwrite(bitmapBits, width * height * 3, 1, file);

    fclose(file);

    printf("Screenshot saved to screenshot.bmp\n");

    // Clean up
    free(bitmapBits);
    SelectObject(memDC, oldBitmap);
    DeleteObject(bitmap);
    DeleteDC(memDC);
    ReleaseDC(NULL, screenDC);

    // Socket Initialization
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Failed to initialize Winsock\n");
        return 1;
    }

    // Create a socket
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        printf("Failed to create socket\n");
        WSACleanup();
        return 1;
    }

    // Set up the server address
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
    serverAddress.sin_port = htons(SERVER_PORT);

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        printf("Failed to connect to the server\n");
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // Open the screenshot file in binary mode for reading
    FILE* screenshotFile = fopen("screenshot.bmp", "rb");
    if (screenshotFile == NULL) {
        printf("Failed to open screenshot.bmp\n");
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // Read and send the file data in chunks
    char buffer[4096];
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), screenshotFile)) > 0) {
        if (send(clientSocket, buffer, bytesRead, 0) == SOCKET_ERROR) {
            printf("Failed to send file data\n");
            fclose(screenshotFile);
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }
    }

    fclose(screenshotFile);
    closesocket(clientSocket);
    WSACleanup();

    printf("Screenshot sent to the server\n");

    return 0;
}
