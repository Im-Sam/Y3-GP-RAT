#include <stdio.h>
#include <windows.h>
#include <urlmon.h>

#pragma comment(lib, "urlmon.lib")

void createDirectory(const char *directory)
{
    if (!CreateDirectory(directory, NULL))
    {
        if (GetLastError() == ERROR_PATH_NOT_FOUND)
        {
            printf("Failed to create directory: Path not found\n");
        }
        else
        {
            printf("Failed to create directory: Error %d\n", GetLastError());
        }
    }
    else
    {
        printf("Directory created successfully.\n");
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <url>\n", argv[0]);
        return 1;
    }

    const char *url = argv[1]; // Get the URL from command-line argument

    // Destination directory to save the downloaded file
    const char *directory = "downloads";

    // Create the destination directory if it doesn't exist
    createDirectory(directory);

    // Extract the file name from the URL
    const char *filename = strrchr(url, '/');
    if (filename != NULL)
    {
        filename++; // Skip the '/'
    }
    else
    {
        // Use a default filename if the URL doesn't provide one
        filename = "downloaded_file";
    }

    // Construct the full file path
    char filepath[MAX_PATH];
    sprintf(filepath, "%s\\%s", directory, filename);

    // Download the file using the URLDownloadToFile function
    HRESULT hr = URLDownloadToFile(NULL, url, filepath, 0, NULL);
    if (hr == S_OK)
    {
        printf("File downloaded successfully.\n");
    }
    else
    {
        printf("Download failed: HRESULT 0x%08lx\n", hr);
    }

    return 0;
}
