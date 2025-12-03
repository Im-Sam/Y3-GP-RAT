# Windows Remote Access Trojan

## Project Overview
This repository contains the source code for a Year 3 academic project demonstrating low-level system programming on Windows. The project explores the implementation of client-server architectures using C for the client-side interactions and Python for the server-side handling.

The primary objective was to understand how operating system APIs can be utilized for system monitoring, input handling, and background service management by a malicious actor. 

## Technical Architecture

The project consists of several independent modules, each demonstrating a specific area of the Windows API (WinAPI) and Winsock2 library.

### 1. Network Communication (Winsock2)
*   **Implementation:** `file.c`, `clipboard.c`, `keylogger.c`, `screenshot.c`
*   **Concept:** Raw TCP socket programming.
*   **Details:** The clients utilize the `winsock2.h` library to establish TCP streams. The implementation covers socket initialization (`WSAStartup`), connection establishment (`connect`), and data transmission (`send`/`recv`) without relying on high-level HTTP libraries.

### 2. Windows Hooks
*   **Implementation:** `keylogger.c`
*   **Concept:** Global Input Hooks.
*   **Details:** Utilizes `SetWindowsHookEx` with `WH_KEYBOARD_LL` to install a hook procedure that monitors low-level keyboard input events. This demonstrates how the Windows message loop processes input before it reaches the target application.

### 3. Service Control Manager
*   **Implementation:** `reverse.c`
*   **Concept:** Windows Services and Persistence.
*   **Details:** Implements the `ServiceMain` and `ControlHandler` functions required to interface with the Service Control Manager (SCM). The program registers itself as a background service using `StartServiceCtrlDispatcher`, allowing it to run independent of active user sessions.

### 4. Graphics Device Interface (GDI)
*   **Implementation:** `screenshot.c`
*   **Concept:** Screen Capture and Bitmap Manipulation.
*   **Details:** Interacts with the GDI to acquire a handle to the device context (`GetDC`) and creates a compatible memory bitmap. It uses `BitBlt` to copy pixel data from the screen buffer to memory and constructs a standard BMP file header for export.

### 5. Multi-threaded Control Server
*   **Implementation:** `controller.py`
*   **Concept:** Concurrency and C2 Logic.
*   **Details:** A Python-based multi-threaded server that manages concurrent connections. It uses Python's `threading` and `queue` modules to handle incoming connections and operator input simultaneously.

## Technologies

*   **Languages:** C, Python 3
*   **Libraries:**
    *   **Windows:** `windows.h`, `winsock2.h`, `winuser.h`, `urlmon.h`
    *   **Python:** `socket`, `threading`, `struct`, `os`
*   **Tools:** MinGW/GCC (for compilation)

## Disclaimer
This software was developed for educational purposes as part of a university research project. It is intended to demonstrate technical proficiency in C and Windows API programming.
